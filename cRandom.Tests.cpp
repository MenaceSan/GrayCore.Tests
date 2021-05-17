//
//! @file cRandom.Tests.cpp
//
#include "pch.h"
#include "cRandom.h"

namespace Gray
{
	UNITTEST_CLASS(cRandom)
	{
		UNITTEST_METHOD(cRandom)
		{
			// Test general randomness.
			cRandomOS& rSys = cRandomOS::I();

			BYTE s1[16];
			rSys.GetNoise(s1, sizeof(s1));

			g_Rand.InitSeedOS();
			UINT u1 = g_Rand.get_RandUns();
			UINT u2 = g_Rand.get_RandUns();
			UNITTEST_TRUE(u2 != u1);

			BYTE s2[16];
			rSys.GetNoise(s2, sizeof(s2));
			UNITTEST_TRUE(s2 != s1);

			// Test seed repeatability.
			cRandomBase::SEED_t uSeed = 123;
			g_Rand.InitSeed(&uSeed, sizeof(uSeed));
			u1 = g_Rand.get_RandUns();
#ifdef __linux__	// GNUC
			UNITTEST_TRUE(u1 == 3711425268);
#else
			UNITTEST_TRUE(u1 == 595497326);	// this will vary on other systems ?
#endif

			g_Rand.InitSeed(s2, sizeof(s2));
			u2 = g_Rand.get_RandUns();
			UNITTEST_TRUE(u1 != u2);

			g_Rand.InitSeed(&uSeed, sizeof(uSeed));
			u2 = g_Rand.get_RandUns();
			UNITTEST_TRUE(u1 == u2);	// Must repeat.	
		}
	};
	UNITTEST2_REGISTER(cRandom, UNITTEST_LEVEL_Core);
}
