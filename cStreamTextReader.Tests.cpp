//! @file cTextReader.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cFileText.h>
#include <GrayCore/include/cMime.h>
#include <GrayCore/include/cTextReader.h>

namespace Gray {
struct UNITTEST_N(cTextReader) : public cUnitTest {
    const FILECHAR_t* k_TestFileIni = _FN("cIniFileUnitTest") _FN(MIME_EXT_ini);  // static

    UNITTEST_METHOD(cTextReader) {
        //! test reading cTextReader.
        //! @note any text changes to this file can invalidate the test results.

        cStringF sFilePath = cFilePath::CombineFilePathX(get_TestInpDir(), k_TestFileIni);

        static const int k_MaxLineLen = 180;  // was cStream::k_FILE_BLOCK_SIZE 256. Assume no other line is this long for my test.

        cFileTextReader tr(k_MaxLineLen);
        HRESULT hRes = tr.OpenX(sFilePath, OF_READ | OF_TEXT | OF_SHARE_DENY_NONE | OF_CACHE_SEQ);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        int iLineNumber = 1;  // 1 based.
        for (;;) {
            char line[StrT::k_LEN_Default];
            hRes = tr.ReadStringLine(TOSPAN(line));
            UNITTEST_TRUE(SUCCEEDED(hRes));
            if (hRes == 0) break;

            //*** Make this over k_MaxLineLen chars long ****************************************************************************************************************************************************

            if (hRes >= k_MaxLineLen) {
                // Warning  line length was too long !
                UNITTEST_TRUE(iLineNumber == 167);  // Fix this if source changes.
                DEBUG_MSG(("line %d length was > %d", iLineNumber, hRes));
            } else {
                iLineNumber++;  // don't count split lines.
            }

            UNITTEST_TRUE(hRes <= k_MaxLineLen);
        }

        UNITTEST_TRUE(iLineNumber == 57);  // Fix this if source changes.
        UNITTEST_TRUE(tr.get_CurrentLineNumber() == iLineNumber);
    }
};
UNITTEST2_REGISTER(cTextReader, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
