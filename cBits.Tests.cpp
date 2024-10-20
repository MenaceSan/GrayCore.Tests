//! @file cBits.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cBits.h>
#include <GrayCore/include/cTriState.h>

namespace Gray {
struct UNITTEST_N(cBits) : public cUnitTest {
    UNITTEST_METHOD(cBits) {
        const int i1 = -123;  // test shifting signed numbers.
        const int i2 = i1 >> 2;
        STATIC_ASSERT(i2 == -31, i2);
        const int i3 = i1 << 2;
        STATIC_ASSERT(i3 == -492, i3);

        //! bit reversal.
        BYTE u8 = cBits::Reverse<BYTE>(0x18);
        UNITTEST_TRUE(u8 == 0x18);
        u8 = cBits::Reverse<BYTE>(0x34);
        UNITTEST_TRUE(u8 == 0x2c);

        WORD u16 = cBits::Reverse<WORD>((WORD)0x1234);
        UNITTEST_TRUE(u16 == 0x2c48);

        UINT32 u32 = cBits::Reverse<UINT32>(0x12345678UL);
        UNITTEST_TRUE(u32 == 0x1e6a2c48);
        ULONG ul = cBits::Reverse<ULONG>(0x12345678UL);
        UNITTEST_TRUE(ul == 0x1e6a2c48);

        cTriState triX;
        cTriState tri0(false);
        cTriState tri1(true);

        UNITTEST_TRUE(triX != tri0);
        UNITTEST_TRUE(tri0 != triX);
        UNITTEST_TRUE(tri1 != triX);

        UNITTEST_TRUE(triX == BITOP_t::_Toggle);
        UNITTEST_TRUE(tri0.get_Bool() == false);
        UNITTEST_TRUE(tri1.get_Bool() == true);

        UNITTEST_TRUE(cBits::Highest1Bit<UINT32>(0) == 0);
        UNITTEST_TRUE(cBits::Highest1Bit<UINT32>(1) == 1);
        UNITTEST_TRUE(cBits::Highest1Bit<UINT32>(4095) == 12);
        UNITTEST_TRUE(cBits::Highest1Bit<UINT32>(4096) == 13);
        UNITTEST_TRUE(cBits::Highest1Bit<UINT32>(4196) == 13);

        UNITTEST_TRUE(cBits::Lowest1Bit<UINT64>(4096) == 13);
        UNITTEST_TRUE(cBits::Lowest1Bit<UINT64>(4095) == 1);

        UNITTEST_TRUE(cBits::Highest1Bit<UINT64>(0) == 0);
        UNITTEST_TRUE(cBits::Highest1Bit<UINT64>(1) == 1);
        UNITTEST_TRUE(cBits::Highest1Bit<UINT64>(4095) == 12);
        UNITTEST_TRUE(cBits::Highest1Bit<UINT64>(4096) == 13);
        UNITTEST_TRUE(cBits::Highest1Bit<UINT64>(4196) == 13);

        UNITTEST_TRUE(cBits::Lowest1Bit<UINT32>(4096) == 13);
        UNITTEST_TRUE(cBits::Lowest1Bit<UINT32>(4095) == 1);

        UNITTEST_TRUE(cBits::Highest1Bit(12) == 4);

 
    }  // namespace Gray
};
UNITTEST2_REGISTER(cBits, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
