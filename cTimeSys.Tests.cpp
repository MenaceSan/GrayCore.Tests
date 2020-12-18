//
//! @file cTimeSys.Tests.cpp
//
#include "pch.h"
#include "cTimeSys.h"
#include "cThreadLock.h"
#include "cLogMgr.h"

namespace Gray
{
	UNITTEST2_CLASS(cTimeSys)
	{
		void TestTimerPerf()
		{
			//
			// Allow one failure for possible counter wrapping.
			// On a 4Ghz 32-bit machine the cycle counter wraps about once per second;
			// since the whole test is about 10ms, it shouldn't happen twice in a row.
			//
			int hardfail = 0;
			bool bSuccess = false;
			while (!bSuccess)
			{
				UNITTEST2_TRUE(hardfail <= 1);

				// Get a reference ratio cycles/ms
				TIMEPERF_t ratio = 0;

				// Check that the ratio is mostly constant
				bSuccess = true;
				for (unsigned long millisecs = 1; millisecs <= 4; millisecs++)
				{
					cTimePerf tStart(true);
					unsigned long iCount = cTimeSys::WaitSpin(millisecs);
					UNITTEST2_TRUE(iCount > 0);

					TIMEPERF_t cycles = tStart.get_AgePerf();
					if (ratio <= 0)
					{
						ratio = cycles / millisecs;
						continue;
					}
					// Allow variation up to 20%
					if (cycles / millisecs < ratio - ratio / 5 ||
						cycles / millisecs > ratio + ratio / 5)
					{
						hardfail++;
						ratio = 0;
						bSuccess = false;
						break;
					}
				}
			}
		}

		UNITTEST2_METHOD(cTimeSys)
		{
			cTimePerf::InitFreq();

			cTimePerf tStart0(true);
			cTimeSys tNow = cTimeSys::GetTimeNow();
			TIMEPERF_t tDiff0 = tStart0.get_AgePerf();
			UNITTEST2_TRUE(tDiff0 >= 0);	// e.g. 2	// NOTE This can be so fast it looks like 0 ?
			UNITTEST2_TRUE(tNow.get_TimeSys() >= 1);

			cTimePerf tStart1(true);
			cThreadId::SleepCurrent(0);
			TIMEPERF_t tDiff1 = tStart1.get_AgePerf();
			UNITTEST2_TRUE(tDiff1 >= 1);

			cTimePerf tStart2(true);
			cThreadId::SleepCurrent(1);
			TIMEPERF_t tDiff2 = tStart2.get_AgePerf();
			UNITTEST2_TRUE(tDiff2 > 1);

			const TIMESYSD_t k_nLen = 200;
			const TIMESYSD_t k_nLenVar = 20;	// Allow variation up to 10%

			for (int tocks = 1; tocks <= 3; tocks++)
			{
				cTimeSys hires = cTimeSys::GetTimeNow();
				cThreadId::SleepCurrent((int)(k_nLen * tocks));
				TIMESYSD_t nAge = hires.get_AgeSys();
				UNITTEST2_TRUE(nAge >= (k_nLen - k_nLenVar) * tocks);
				UNITTEST2_TRUE(nAge <= (k_nLen + k_nLenVar) * tocks);
			}
		}
	};
	UNITTEST2_REGISTER(cTimeSys, UNITTEST_LEVEL_Core);
}
