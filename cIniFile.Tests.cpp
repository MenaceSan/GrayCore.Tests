//! @file cIniFile.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cFilePath.h>
#include <GrayCore/include/cIniFile.h>
#include <GrayCore/include/cMime.h>

namespace Gray {
struct UNITTEST_N(cIniFile) : public cUnitTest {
    const FILECHAR_t* k_TestFileIni = _FN("cIniFileUnitTest") _FN(MIME_EXT_ini);  // static

    HRESULT UnitTest_Section(const cIniSection* pSection) {
        //! k_asTextLines
        //! dump contexts of a section.
        ASSERT_NN(pSection);
        ITERATE_t iLines = 0;
        for (iLines = 0; iLines < pSection->get_LineQty(); iLines++) {
            const IniChar_t* pszLine = pSection->GetLineEnum(iLines);
            UNITTEST_TRUE(pszLine != nullptr);
            // DEBUG_MSG(( "%s", LOGSTR(pszLine) ));
        }
        return (HRESULT)iLines;
    }

    UNITTEST_METHOD(cIniFile) {
        //! read and write tests.
        //! open some INI file.

        cUnitTests& uts = cUnitTests::I();
        cStringF sTestInpFile = cFilePath::CombineFilePathF(cFilePath::k_DirSep, get_TestInpDir(), k_TestFileIni, nullptr);

        cIniFile file;
        HRESULT hRes = file.ReadIniFile(sTestInpFile);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        // read headless section first. has no [SECTION] header.
        cRefPtr<cIniSectionEntry> pSection = file.FindSection(nullptr);
        if (pSection != nullptr) {
            UnitTest_Section(pSection);
        }

        for (ITERATE_t i = 0;; i++) {
            auto pSection2 = file.EnumSection(i);
            if (pSection2 == nullptr) break;
            pSection = file.FindSection(pSection2->get_SectionTitle(), false);
            UNITTEST_TRUE(pSection != nullptr);
            UnitTest_Section(pSection);
        }

        // make a change to the INI file.
        const IniChar_t* pszTestSection = "TestSection";
        const IniChar_t* pszTestKey = "TestKey";
        const IniChar_t* pszTestLine = "TestKey=9";
        // const IniChar_t* pszTestLine2 = "TestKey2:3";

        const IniChar_t* pszLine = file.FindKeyLinePtr(pszTestSection, pszTestKey);
        file.SetKeyLine(pszTestSection, pszTestKey, pszTestLine);

        // now write it back out.
        cStringF sIniWriteFile = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("IniFileUnitTest") _FN(MIME_EXT_ini));
        hRes = file.WriteIniFile(sIniWriteFile);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        // read it back again to make sure its correct ?
        cIniFile file2;
        hRes = file2.ReadIniFile(sIniWriteFile);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        pszLine = file2.FindKeyLinePtr(pszTestSection, pszTestKey);
        UNITTEST_TRUE(pszLine != nullptr);
        UNITTEST_TRUE(!StrT::Cmp(pszLine, pszTestLine));

#if 0
			filewrite.WriteString(
				"//\n"
				"// CScriptTest.scp\n"
				"// This is a test script to read and see if it is read correctly.\n"
				"//\n"
				"\n");

			filewrite.WriteKeyStrQ("primaryfileprop", "123asd   ");

			filewrite.WriteSectionHead1("SECTION", "1");
			filewrite.WriteKeyStrQ("SECTION1Key", "SECTION1Keydata");

			filewrite.WriteSectionHead1("SECTION", "b2");
			filewrite.WriteKeyInt("SECTIONb2KeyValue", 123);
			filewrite.WriteKeyInt("SECTIONb2KeyValue123", 123);

			filewrite.WriteString("bbb=has a comment at the end of the line // sdf\n"); // comment at end of line.

			filewrite.WriteSectionHead1("SECTION", "c3isempty");
#endif
    }
};
UNITTEST2_REGISTER(cIniFile, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
