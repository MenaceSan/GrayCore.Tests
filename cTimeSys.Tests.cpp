//
//! @file cTimeSys.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cLogMgr.h>
#include <GrayCore/include/cThreadLock.h>
#include <GrayCore/include/cTimeSys.h>

namespace Gray {
struct UNITTEST_N(cTimeSys) : public cUnitTest {
    void TestTimerPerf() {
        //
        // Allow one failure for possible counter wrapping.
        // On a 4Ghz 32-bit machine the cycle counter wraps about once per second;
        // since the whole test is about 10ms, it shouldn't happen twice in a row.
        //
        int hardfail = 0;
        bool bSuccess = false;
        while (!bSuccess) {
            UNITTEST_TRUE(hardfail <= 1);

            // Get a reference ratio cycles/ms
            TIMEPERF_t ratio = 0;

            // Check that the ratio is mostly constant
            bSuccess = true;
            for (unsigned long milliSec = 1; milliSec <= 4; milliSec++) {
                cTimePerf tStart(true);
                unsigned long iCount = cTimeSys::WaitSpin(milliSec);
                UNITTEST_TRUE(iCount > 0);

                TIMEPERF_t cycles = tStart.get_AgePerf();
                if (ratio <= 0) {
                    ratio = cycles / milliSec;
                    continue;
                }
                // Allow variation up to 20%
                if (cycles / milliSec < ratio - ratio / 5 || cycles / milliSec > ratio + ratio / 5) {
                    hardfail++;
                    ratio = 0;
                    bSuccess = false;
                    break;
                }
            }
        }
    }

    UNITTEST_METHOD(cTimeSys) {
        cTimePerf::InitFreq();

        cTimePerf tStart0(true);
        cTimeSys tNow = cTimeSys::GetTimeNow();
        TIMEPERF_t tDiff0 = tStart0.get_AgePerf();
        UNITTEST_TRUE(tDiff0 >= 0);  // e.g. 2	// NOTE This can be so fast it looks like 0 ?
        UNITTEST_TRUE(tNow.get_TimeSys() >= 1);

        cTimePerf tStart1(true);
        cThreadId::SleepCurrent(0);
        TIMEPERF_t tDiff1 = tStart1.get_AgePerf();
        UNITTEST_TRUE(tDiff1 >= 1);

        cTimePerf tStart2(true);
        cThreadId::SleepCurrent(1);
        TIMEPERF_t tDiff2 = tStart2.get_AgePerf();
        UNITTEST_TRUE(tDiff2 > 1);

        const TIMESYSD_t k_nLen = 200;  // sleep units.

        for (int tocks = 1; tocks <= 3; tocks++) {
            const TIMESYSD_t nTimeToSleep = k_nLen * tocks;

            const cTimeSys timeStart = cTimeSys::GetTimeNow();
            cThreadId::SleepCurrent(nTimeToSleep);
            const TIMESYSD_t nTimeSlept = timeStart.get_AgeSys();

            UNITTEST_TRUE(nTimeSlept >= nTimeToSleep - 10);     // never sleep less !? but allow a little slop
            UNITTEST_TRUE(nTimeSlept <= k_nLen * (tocks + 1));  //  we can sleep longer ? system lag.
        }
    }
};
UNITTEST2_REGISTER(cTimeSys, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
