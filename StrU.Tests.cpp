//
//! @file StrU.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/StrU.h>

namespace Gray {
struct UNITTEST_N(StrU) : public cUnitTest {
    static bool GRAYCALL UnitTestU(const wchar_t* pwText, StrLen_t nLen) {
        // Does the input Unicode wchar_t string match its equivalent UTF8 string ?
        if (nLen <= k_StrLen_UNK) nLen = StrT::Len(pwText);

        char szTmp8[1024];
        const StrLen_t iLen8 = StrU::UNICODEtoUTF8(szTmp8, STRMAX(szTmp8), pwText, nLen);
        if (iLen8 <= 0)  // should be same or larger than nLen
            return false;

        wchar_t wTmpU[1024];
        if (iLen8 >= STRMAX(wTmpU)) return false;
        const StrLen_t iLenU = StrU::UTF8toUNICODE(wTmpU, STRMAX(wTmpU), szTmp8, iLen8);
        if (iLenU != nLen) return false;
        if (StrT::Cmp<wchar_t>(wTmpU, pwText))  // back to original text?
            return false;
        return true;
    }

    static bool GRAYCALL UnitTest8(const char* pszText, StrLen_t nLen) {
        // Does the input UTF8 / ASCII string match its equivalent Unicode string ?

        if (nLen <= k_StrLen_UNK) nLen = StrT::Len(pszText);

        wchar_t wTmpU[1024];
        if (nLen >= STRMAX(wTmpU)) return false;
        const StrLen_t iLenU = StrU::UTF8toUNICODE(wTmpU, STRMAX(wTmpU), pszText, nLen);
        if (iLenU <= 0)  // should be same or smaller than nLen
            return false;

        char szTmp8[1024];
        const StrLen_t iLen8 = StrU::UNICODEtoUTF8(szTmp8, STRMAX(szTmp8), wTmpU, iLenU);
        if (iLen8 != nLen) return false;
        if (StrT::Cmp<char>(szTmp8, pszText))  // back to original text?
            return false;
        return true;
    }

    static void GRAYCALL TryBadU() {
        // try to construct a 'bad' UNICODE string. is there such a thing?

        wchar_t badU[StrT::k_LEN_Default];
        for (int i = 0; i < STRMAX(badU); i++) {
            badU[i] = 1 + ((0xFFFE * i) / (STRMAX(badU) - 1));
        }
        badU[STRMAX(badU)] = '\0';

        UNITTEST_TRUE(UnitTestU(badU, k_StrLen_UNK));
    }

    UNITTEST_METHOD(StrU) {
        // TryBadU();

        // https://www.cl.cam.ac.uk/~mgk25/ucs/examples/quickbrown.txt
        static const wchar_t* kGreekU = L"Σὲ γνωρίζω ἀπὸ τὴν κόψη";
        static const char* kGreek8 = "Î£á½² Î³Î½Ï‰Ïá½·Î¶Ï‰ á¼€Ï€á½¸ Ï„á½´Î½ Îºá½¹ÏˆÎ·";  // in UTF8

        UNITTEST_TRUE(UnitTestU(kGreekU, k_StrLen_UNK));

        UNITTEST_TRUE(UnitTest8(k_sTextBlob, k_TEXTBLOB_LEN));
        UNITTEST_TRUE(UnitTest8(kGreek8, k_StrLen_UNK));
    }
};
UNITTEST2_REGISTER(StrU, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
