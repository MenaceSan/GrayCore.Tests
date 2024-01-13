//
//! @file cFloat.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cArray.h>
#include <GrayCore/include/cFloat.h>
#include <GrayCore/include/cTypes.h>

namespace Gray {
struct UNITTEST_N(cFloat) : public cUnitTest {
    UNITTEST_METHOD(cFloat) {
        // Test basic assumptions about floats.
        cFloat32 f1(65536);
        cFloat32 f2;
        f2.m_v.u_f = 65536;
        UNITTEST_TRUE(f1.m_v.u_f == f2.m_v.u_f);

        f1.put_Bits(cFloat32::k_uOne);
        UNITTEST_TRUE(1.0f == f1.m_v.u_f);
        f2.put_Bits(cFloat32::k_uTwo);
        UNITTEST_TRUE(2.0f == f2.m_v.u_f);

        cFloat64 d1(65536);
        cFloat64 d2;
        d2.m_v.u_d = 65536;
        UNITTEST_TRUE(d1.m_v.u_d == d2.m_v.u_d);

        cFloat64 dx1(cTypeLimit<double>::Max());
        UNITTEST_TRUE(dx1.m_v.u_d > 10);
        cFloat64 dx2(cTypeLimit<double>::Min());
        UNITTEST_TRUE(dx2.m_v.u_d < -10);
    }
};
UNITTEST2_REGISTER(cFloat, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
