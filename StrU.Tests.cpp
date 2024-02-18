//! @file StrU.Tests.cpp

#include "pch.h"
#include <GrayCore/include/StrU.h>
#include <GrayCore/include/cString.h>

namespace Gray {
struct UNITTEST_N(StrU) : public cUnitTest {
    static bool GRAYCALL UnitTestU(const cSpan<wchar_t>& src) {
        // Does the input Unicode wchar_t string match its equivalent UTF8 string ?

        const StrLen_t lenA1 = StrU::UNICODEtoUTF8Size(src);
        if (lenA1 <= 1) return false;
        if (lenA1 < src.GetSize()) return false;  // should be same or larger than nLen

        char szTmpA[1024];
        if (lenA1 >= STRMAX(szTmpA)) return false;
        const StrLen_t lenA = StrU::UNICODEtoUTF8(TOSPAN(szTmpA), src);
        if (lenA != lenA1) return false;

        wchar_t wTmpU[1024];
        if (src.GetSize() >= STRMAX(wTmpU)) return false;
        const StrLen_t iLenU = StrU::UTF8toUNICODE(TOSPAN(wTmpU), ToSpan(szTmpA, lenA));
        if (iLenU != src.GetSize()) return false;
        if (StrT::Cmp<wchar_t>(wTmpU, src))  // back to original text?
            return false;
        return true;
    }

    static bool GRAYCALL UnitTestA(const cSpan<char>& src) {
        // Does the input UTF8 / ASCII string match its equivalent Unicode string ?

        const StrLen_t iLenU1 = StrU::UTF8toUNICODELen(src);
        if (iLenU1 <= 1) return false;
        if (iLenU1 > src.GetSize()) return false;  // should be same or smaller than nLen

        wchar_t wTmpU[1024];
        if (iLenU1 >= STRMAX(wTmpU)) return false;
        const StrLen_t iLenU = StrU::UTF8toUNICODE(TOSPAN(wTmpU), src);
        if (iLenU != iLenU1) return false;

        char szTmpA[1024];
        if (src.GetSize() >= STRMAX(szTmpA)) return false;
        const StrLen_t lenA = StrU::UNICODEtoUTF8(TOSPAN(szTmpA), ToSpan(wTmpU, iLenU));
        if (lenA != src.GetSize()) return false;
        if (StrT::Cmp<char>(szTmpA, src))  // back to original text?
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

        UNITTEST_TRUE(UnitTestU(StrT::ToSpanStr(badU)));
    }

    UNITTEST_METHOD(StrU) {
        // TryBadU();

        // https://www.cl.cam.ac.uk/~mgk25/ucs/examples/quickbrown.txt
        static const wchar_t kGreekU[] = L"Σὲ γνωρίζω ἀπὸ τὴν κόψη";
        static const char kGreekA[] = "Î£á½² Î³Î½Ï‰Ïá½·Î¶Ï‰ á¼€Ï€á½¸ Ï„á½´Î½ Îºá½¹ÏˆÎ·";  // in UTF8

        cStringW sU = kGreekU;
        cStringW sU2 = kGreekA;
        UNITTEST_TRUE(sU == sU2);

        cStringA sA = kGreekA;
        cStringA sA2 = kGreekU;
        UNITTEST_TRUE(sA == sA2);

        UNITTEST_TRUE(UnitTestU(TOSPAN_LIT(kGreekU)));
        UNITTEST_TRUE(UnitTestA(TOSPAN_LIT(kGreekA)));

        UNITTEST_TRUE(UnitTestU(ToSpan<wchar_t>(k_sTextBlob)));
        UNITTEST_TRUE(UnitTestA(ToSpan<char>(k_sTextBlob)));
    }
};
UNITTEST2_REGISTER(StrU, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
