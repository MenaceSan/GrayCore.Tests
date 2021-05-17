// 
//! @file cTimeFile.Tests.cpp
//
#include "pch.h"
#include "cTimeFile.h"

namespace Gray
{
	UNITTEST_CLASS(cTimeFile)
	{
		UNITTEST_METHOD(cTimeFile)
		{
			UINT64 uVal = 0xFFFFFFFFFFFFFFFFULL;
			uVal /= cTimeFile::k_nFreq;	// seconds.
			uVal /= 60 * 60 * 24;		// days.
			uVal /= 365;	// years

			UNITTEST_TRUE(uVal == 58494);
		}
	};
	UNITTEST2_REGISTER(cTimeFile, UNITTEST_LEVEL_Core);
}

