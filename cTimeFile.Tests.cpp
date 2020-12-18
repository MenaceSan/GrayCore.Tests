// 
//! @file cTimeFile.Tests.cpp
//
#include "pch.h"
#include "cTimeFile.h"

namespace Gray
{
	UNITTEST2_CLASS(cTimeFile)
	{
		UNITTEST2_METHOD(cTimeFile)
		{
			UINT64 uVal = 0xFFFFFFFFFFFFFFFFULL;
			uVal /= cTimeFile::k_nFreq;	// seconds.
			uVal /= 60 * 60 * 24;		// days.
			uVal /= 365;	// years

			UNITTEST2_TRUE(uVal == 58494);
		}
	};
	UNITTEST2_REGISTER(cTimeFile, UNITTEST_LEVEL_Core);
}

