//! @file cFileText.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cFileText.h>
#include <GrayCore/include/cMime.h>

#if USE_CRT

namespace Gray {
struct UNITTEST_N(cFileText) : public cUnitTest {
    HRESULT TestWrite(cFile& file) {
        HRESULT hRes = file.WriteString(cUnitTests::k_sTextBlob.get_CPtr());
        UNITTEST_TRUE(SUCCEEDED(hRes));
        file.Close();
        return hRes;
    }

    UNITTEST_METHOD(cFileText) {
        //! @todo test reading cFileText and seek position inside it. Must not be fooled by \r\n and \n.
        cUnitTests& uts = cUnitTests::I();

        cTextPos fp1(cTextPos::k_Invalid);
        fp1 = cTextPos::k_Zero;

        // write a file OF_BINARY.
        cStringF sFilePathB = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("CFileTextB") _FN(MIME_EXT_txt));
        cFile testfileB;
        HRESULT hRes = testfileB.OpenX(sFilePathB, OF_CREATE | OF_WRITE | OF_BINARY);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        hRes = TestWrite(testfileB);
        // UNITTEST_TRUE(hRes == cUnitTests::k_sTextBlob._Len);

        // Write OF_TEXT
        cStringF sFilePathT = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("CFileTextT") _FN(MIME_EXT_txt));
        cFileText testfileT;
        hRes = testfileT.OpenX(sFilePathT, OF_CREATE | OF_WRITE | OF_TEXT);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        hRes = TestWrite(testfileT);
        // UNITTEST_TRUE(hRes == cUnitTests::k_sTextBlob._Len);

        // Read it back.
        cFileText testfileR;
        hRes = testfileR.OpenX(sFilePathT, OF_READ | OF_TEXT);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        int nLines = 0;
        for (;; nLines++) {
            cStringA s;
            hRes = testfileR.ReadStringLineA(s);
            if (hRes == 0)  // the end.
                break;
            UNITTEST_TRUE(SUCCEEDED(hRes));
        }
    }
};
UNITTEST2_REGISTER(cFileText, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray

#endif  // USE_CRT
