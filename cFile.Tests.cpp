//
//! @file cFile.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cFile.h>
#include <GrayCore/include/cMime.h>

namespace Gray {
GRAYCORE_TEST_LINK void GRAYCALL UnitTest_Write(cStreamOutput& testfile1)  // static
{
    //! Write strings to cStreamOutput.
    for (ITERATE_t i = 0; !cUnitTests::k_asTextLines[i].isNull(); i++) {
        HRESULT hRes = testfile1.WriteString(cUnitTests::k_asTextLines[i].get_CPtr());
        UNITTEST_TRUE(SUCCEEDED(hRes));
        testfile1.WriteString(_GT(STR_NL));
    }
}

GRAYCORE_TEST_LINK void GRAYCALL UnitTest_Read(cStreamInput& stmIn, bool bString)  // static
{
    //! Read strings from cStreamInput (as binary or text).
    //! Other side of UnitTest_Write()

    GChar_t szTmp[256];

    for (ITERATE_t j = 0; !cUnitTests::k_asTextLines[j].isNull(); j++) {
        const GChar_t* pszLine = cUnitTests::k_asTextLines[j];
        StrLen_t iLenStr = StrT::Len(pszLine);
        UNITTEST_TRUE(iLenStr < (StrLen_t)STRMAX(szTmp));
        size_t nSizeBytes = (iLenStr + 1) * sizeof(GChar_t);
        HRESULT hResRead = bString ? stmIn.ReadStringLine(szTmp, STRMAX(szTmp)) : stmIn.ReadX(szTmp, nSizeBytes);
        UNITTEST_TRUE(hResRead == (HRESULT)(bString ? (iLenStr + 1) : nSizeBytes));
        UNITTEST_TRUE(cMem::IsEqual(szTmp, pszLine, iLenStr * sizeof(GChar_t)));  // pszLine has no newline.
        UNITTEST_TRUE(szTmp[iLenStr] == '\n');
    }

    // Check for proper read past end of file.
    HRESULT hResRead = stmIn.ReadX(szTmp, STRMAX(szTmp));
    UNITTEST_TRUE(hResRead == 0);
    hResRead = stmIn.ReadX(szTmp, STRMAX(szTmp));
    UNITTEST_TRUE(hResRead == 0);
}

struct UNITTEST_N(cFile) : public cUnitTest {
    void TestWrite(cStringF sFilePath) {
        //
        cFile testfile1;
        HRESULT hRes = testfile1.OpenX(sFilePath, OF_CREATE | OF_WRITE | OF_BINARY);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        UnitTest_Write(testfile1);
    }

    void TestRead(cStringF sFilePath, bool isString) {
        cFile testfile2;
        HRESULT hRes = testfile2.OpenX(sFilePath, isString ? (OF_READ | OF_TEXT) : (OF_READ | OF_BINARY));
        UNITTEST_TRUE(SUCCEEDED(hRes));

        cFileStatus filestatus1;
        hRes = testfile2.GetFileStatus(filestatus1);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        UnitTest_Read(testfile2, isString);
    }

    void TestGrow(cStringF sFilePath) {
        // open and Grow a file.

        cFile testfile2;
        HRESULT hRes = testfile2.OpenX(sFilePath, OF_READ | OF_WRITE | OF_BINARY);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        cFileStatus filestatus1;
        hRes = testfile2.GetFileStatus(filestatus1);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        // grow
        auto lengthFile = filestatus1.GetFileLength();  // 84
        testfile2.SetLength(CastN(STREAM_POS_t, lengthFile + 1024));
        hRes = HResult::GetLast();
        UNITTEST_TRUE(SUCCEEDED(hRes));

        testfile2.Close();

        cFileStatus filestatus2;
        hRes = filestatus2.ReadFileStatus(sFilePath);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        UNITTEST_TRUE(filestatus2.GetFileLength() == lengthFile + 1024);
    }

    UNITTEST_METHOD(cFile) {
        //! Create a test file.
        cUnitTests& uts = cUnitTests::I();

        cStringF sFilePath = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("CoreUnitTestFile") _FN(MIME_EXT_txt));
        TestWrite(sFilePath);

        // Read it back.
        TestRead(sFilePath, false);
        TestRead(sFilePath, true);

        TestGrow(sFilePath);

        // Should Fail to delete directory.
        HRESULT hRes = cFile::DeletePath(uts.get_TestOutDir());
        UNITTEST_TRUE(hRes == E_ACCESSDENIED);  // this should fail! E_ACCESSDENIED=WIN32

        hRes = cFile::DeletePath(sFilePath);
        UNITTEST_TRUE(SUCCEEDED(hRes));
    }
};
UNITTEST2_REGISTER(cFile, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
