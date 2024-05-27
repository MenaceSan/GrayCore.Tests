//! @file cRandom.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cRandom.h>

namespace Gray {
struct UNITTEST_N(cRandom) : public cUnitTest {
    UNITTEST_METHOD(cRandom) {
        // Test general randomness.
        cRandomOS& rSys = cRandomOS::I();

        BYTE s1[16];
        rSys.GetNoise(TOSPAN(s1));

        g_Rand.InitSeedOS();
        UINT u1 = g_Rand.get_RandUns();
        UINT u2 = g_Rand.get_RandUns();
        UNITTEST_TRUE(u2 != u1);

        BYTE s2[16];
        rSys.GetNoise(TOSPAN(s2));
        UNITTEST_TRUE(s2 != s1);

        // Test seed repeatability.
        cRandomBase::RAND_t uSeed = 123;
        g_Rand.InitSeed(TOSPANT(uSeed));
        u1 = g_Rand.get_RandUns();
        UNITTEST_TRUE(u1 == 2564704154);

        g_Rand.InitSeed(TOSPAN(s2));
        u2 = g_Rand.get_RandUns();
        UNITTEST_TRUE(u1 != u2);

        g_Rand.InitSeed(TOSPANT(uSeed));
        u2 = g_Rand.get_RandUns();
        UNITTEST_TRUE(u1 == u2);  // Must repeat.
    }
};
UNITTEST2_REGISTER(cRandom, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
