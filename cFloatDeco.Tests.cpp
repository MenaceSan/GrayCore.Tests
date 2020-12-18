// 
//! @file cFloatDeco.Tests.cpp
//
#include "pch.h"
#include "cFloatDeco.h"

namespace Gray
{
	UNITTEST2_CLASS(cFloatDeco)
	{
		UNITTEST2_METHOD(cFloatDeco)
		{
			int nExp10 = 0;
			cFloatDeco v1 = cFloatDeco::GetCachedPower(801, &nExp10);
			UNITTEST2_TRUE(v1.m_uMant == 16905424996341287883);
			UNITTEST2_TRUE(v1.m_iExp2 == -901);
			UNITTEST2_TRUE(nExp10 == 252);
		}
	};
	UNITTEST2_REGISTER(cFloatDeco, UNITTEST_LEVEL_Core);
}
