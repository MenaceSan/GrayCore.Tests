//
//! @file cTimeZone.Tests.cpp
//
#include "pch.h"
#include "cTimeZone.h"

namespace Gray
{
	UNITTEST_CLASS(cTimeZone)
	{
		UNITTEST_METHOD(cTimeZone)
		{
			TZ_TYPE noffset = cTimeZoneMgr::GetLocalTimeZoneOffset();

			const cTimeZone* pTz1 = cTimeZoneMgr::FindTimeZone(TZ_EST);
			UNITTEST_TRUE(pTz1 != nullptr);
		}
	};
	UNITTEST2_REGISTER(cTimeZone, UNITTEST_LEVEL_Core);
}
