//
//! @file StrChar.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/StrChar.h>

namespace Gray {
struct UNITTEST_N(StrChar) : public cUnitTest {
    UNITTEST_METHOD(StrChar) {
        UNITTEST_TRUE(StrChar::IsSpace(' ') == true);
        UNITTEST_TRUE(StrChar::IsSpace('a') == false);
        UNITTEST_TRUE(StrChar::IsSpace('\0') == false);

        STATIC_ASSERT('\n' == 0x0a, Check_NL);  // StrChar::k_NL
        STATIC_ASSERT('\r' == 0x0d, Check_CR);  // StrChar::k_CR

        const wchar_t chNeg = -1;
        UNITTEST_TRUE(chNeg >= 0);  // should not be signed!!

        for (RADIX_t uRadix = StrChar::k_uRadixMin; uRadix <= StrChar::k_uRadixMax; uRadix++) {
            for (UINT uVal = 0; uVal < uRadix; uVal++) {
                char ch = StrChar::U2Radix(uVal, uRadix);
                UINT uVal2 = StrChar::Radix2U(ch, uRadix);
                UNITTEST_TRUE(uVal2 == uVal);
            }

            UNITTEST_TRUE(StrChar::U2Radix(uRadix + 1, uRadix) == '?');
            UNITTEST_TRUE(StrChar::Radix2U('?', uRadix) == uRadix);
        }
    }
};
UNITTEST2_REGISTER(StrChar, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
