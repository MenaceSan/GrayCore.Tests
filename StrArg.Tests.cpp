//
//! @file StrArg.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/StrArg.h>

namespace Gray {
template <typename TYPE>
bool GRAYCALL StrArg_UnitTestT() {
    const TYPE* pszStr1 = StrArg<TYPE>('a', (StrLen_t)10);
    UNITTEST_TRUE(StrT::Cmp<TYPE>(pszStr1, CSTRCONST("aaaaaaaaaa")) == 0);

    const INT32 iVal32 = 0x12;
    const TYPE* pszStr2 = StrArg<TYPE>(iVal32);
    UNITTEST_TRUE(StrT::Cmp<TYPE>(pszStr2, CSTRCONST("18")) == 0);

    const UINT32 uVal32 = 0x12;
    const TYPE* pszStr3 = StrArg<TYPE>(uVal32, (RADIX_t)0x10);
    UNITTEST_TRUE(StrT::Cmp<TYPE>(pszStr3, CSTRCONST("012")) == 0);

    const INT64 iVal64 = 0x12;
    const TYPE* pszStr4 = StrArg<TYPE>(iVal64);
    UNITTEST_TRUE(StrT::Cmp<TYPE>(pszStr4, CSTRCONST("18")) == 0);

    const UINT64 uVal64 = 0x12;
    const TYPE* pszStr5 = StrArg<TYPE>(uVal64, (RADIX_t)0x10);
    UNITTEST_TRUE(StrT::Cmp<TYPE>(pszStr5, CSTRCONST("012")) == 0);

    const double dVal = 123.123;
    const TYPE* pszStr6 = StrArg<TYPE>(dVal);
    UNITTEST_TRUE(StrT::Cmp<TYPE>(pszStr6, CSTRCONST("123.123")) == 0);

    BYTE szTmpB[16];
    szTmpB[0] = 'a';
    szTmpB[1] = 'b';
    szTmpB[2] = 'c';
    szTmpB[3] = '\0';
    const TYPE* pszStrB = StrArg<TYPE>(szTmpB);
    UNITTEST_TRUE(StrT::Cmp<TYPE>(pszStrB, CSTRCONST("abc")) == 0);

    WORD szTmpW[16];
    szTmpW[0] = 'a';
    szTmpW[1] = 'b';
    szTmpW[2] = 'c';
    szTmpW[3] = '\0';
    const TYPE* pszStrW = StrArg<TYPE>(szTmpW);
    UNITTEST_TRUE(StrT::Cmp<TYPE>(pszStrW, CSTRCONST("abc")) == 0);

    return true;
}

struct UNITTEST_N(StrArg) : public cUnitTest {
    UNITTEST_METHOD(StrArg) {
        StrArg_UnitTestT<char>();
        StrArg_UnitTestT<wchar_t>();
    }
};
UNITTEST2_REGISTER(StrArg, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
