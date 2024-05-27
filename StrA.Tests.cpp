//
//! @file StrA.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/StrA.h>

namespace Gray {
struct UNITTEST_N(StrA) : public cUnitTest {
    void TestNamedBitmask() {
        // StrA::MakeNamedBitmask
        static const char* k_aszNames[] = {
            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11",
        };
        char szTmp1[cFilePath::k_MaxLen];
        StrLen_t iRet2 = StrA::MakeNamedBitmask(TOSPAN(szTmp1), 0x123, k_aszNames, _countof(k_aszNames));
        UNITTEST_TRUE(iRet2 >= 7);
        // "0,1,5,8"
    }

    UNITTEST_METHOD(StrA) {
        UNITTEST_TRUE(StrA::IsBoolTrue("true"));
        UNITTEST_TRUE(!StrA::IsBoolTrue("sdf"));
        UNITTEST_TRUE(StrA::IsBoolTrue("1"));
        UNITTEST_TRUE(!StrA::IsBoolTrue("0"));
        UNITTEST_TRUE(StrA::IsBoolTrue("True"));
        UNITTEST_TRUE(!StrA::IsBoolFalse("true"));
        UNITTEST_TRUE(!StrA::IsBoolTrue("false"));
        UNITTEST_TRUE(StrA::IsBoolFalse("false"));

        UNITTEST_TRUE(StrA::IsPlural("hair"));
        UNITTEST_TRUE(!StrA::IsPlural("foot"));
        UNITTEST_TRUE(StrA::IsPlural("feet"));

        TestNamedBitmask();
    }
};

UNITTEST2_REGISTER(StrA, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
