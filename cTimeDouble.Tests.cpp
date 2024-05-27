//! @file cTimeDouble.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cTimeDouble.h>
#include <GrayCore/include/cTimeInt.h>

namespace Gray {
struct UNITTEST_N(cTimeDouble) : public cUnitTest {
    bool UnitTest_1(const cTimeUnits& rTu)  // static
    {
        cTimeUnits Tu2;
        cTimeInt timeTest(rTu);
        timeTest.GetTimeUnits(Tu2, TZ_UTC);
        UNITTEST_TRUE(rTu == Tu2);

        cTimeDouble datetimeTest(rTu);
        datetimeTest.GetTimeUnits(Tu2, TZ_UTC);
        UNITTEST_TRUE(rTu == Tu2);

#if 0
			int iTimeDays = timeTest.GetTime();
			iTimeDays /= cTimeUnits::k_nSecondsPerDay;
			// TIMEDOUBLE_TO_TIME_DAY
			double dDaysDiff = datetimeTest.get_Days() - iTimeDays;
#endif

        // InitTimeDouble()
        cTimeInt timeTest2(datetimeTest);
        UNITTEST_TRUE(timeTest2 == timeTest);

        // GetTimeFromSec()
        cTimeDouble datetimeTest2 = cTimeDouble::GetTimeFromSec(timeTest.GetTime());
        double dSecsDiff = datetimeTest2.get_Days() - datetimeTest.get_Days();
        dSecsDiff = cValT::Abs(dSecsDiff) / cTimeUnits::k_nSecondsPerDay;
        UNITTEST_TRUE(dSecsDiff <= 0.0001);  // close?

#ifdef _WIN32
        // VariantTime is the same as COleDateTime compare
        SYSTEMTIME sysTime;
        rTu.GetSys(sysTime);
        double dTest;
        ::SystemTimeToVariantTime(&sysTime, &dTest);
        UNITTEST_TRUE(dTest == datetimeTest.get_Double());

        ::VariantTimeToSystemTime(dTest, &sysTime);
        UNITTEST_TRUE(rTu == cTimeUnits(sysTime));
#endif
        return true;
    }

    UNITTEST_METHOD(cTimeDouble) {
        cUnitTests& uts = cUnitTests::I();

        cTimeDouble tnow1;
        tnow1.InitTimeNow();
        UNITTEST_TRUE(tnow1.isTimeValid());
        cTimeUnits tn;
        UNITTEST_TRUE(tnow1.GetTimeUnits(tn, TZ_LOCAL));
        UNITTEST_TRUE(tn.isReasonableTimeUnits());

        uts.m_pLog->addDebugInfoF("Current local time is '%s'", LOGSTR(tnow1.GetTimeFormStr(TIMEFORMAT_t::_DEFTZ, TZ_LOCAL)));

        cTimeUnits tu0(1900, 1, 1, 0);  // 0
        // cTimeUnits tu0( 1899,  12, 31, 24 );	// midnight
        cTimeDouble t0(tu0);
        UNITTEST_TRUE(t0.get_Days() == 2);

        cTimeDouble y2k;
        HRESULT hRes = y2k.SetTimeStr(_GT("2000/1/1 00:00:00"), TZ_UTC);
        UNITTEST_TRUE(hRes > 0);
        double dd = y2k.get_Days();
        UNITTEST_TRUE(dd == cTimeDouble::k_nY2K);
        hRes = y2k.SetTimeStr(_GT("2000/1/1 01:00:00"), TZ_UTC);
        UNITTEST_TRUE(hRes > 0);
        dd = y2k.get_Days() - (cTimeDouble::k_nY2K + 1.0 / 24.0);
        UNITTEST_TRUE(cValT::Abs(dd) <= 0.0001);

        // Try a bunch of samples dates to convert back and forth.
        // Valid from 1970 to 2038
        static const cTimeUnits k_sysTimes[] =  // DOW is tested elsewhere
            {
                cTimeUnits(1971, 2, 1, 12),   cTimeUnits(1986, 2, 1, 12), cTimeUnits(2000, 1, 2, 12), cTimeUnits(2004, 8, 2, 12),  // Mon = 38201.5
                cTimeUnits(2004, 12, 30, 12),                                                                                      // Fri = 38251
                cTimeUnits(2005, 1, 1, 12),                                                                                        // Sun = 38253
                cTimeUnits(2005, 12, 28, 12),                                                                                      // Wed = 38713.5
                cTimeUnits(2005, 12, 30, 12),                                                                                      // Fri
                cTimeUnits(2005, 12, 11, 12),                                                                                      // Sat
                cTimeUnits(2006, 1, 1, 12),                                                                                        // Sun
                cTimeUnits(2020, 2, 1, 12),   cTimeUnits(2037, 2, 1, 12),
            };
        for (UINT i = 0; i < _countof(k_sysTimes); i++) {
            UnitTest_1(k_sysTimes[i]);
        }

        cThreadId::SleepCurrent(1);  // may not be accurate enough to advance without sleep?
        cTimeDouble tnow2;
        tnow2.InitTimeNow();
        UNITTEST_TRUE(tnow2.isTimeValid());
        UNITTEST_TRUE(tnow2 > tnow1);

        double dDays = 1 * cTimeUnits::GetUnitDef(TIMEUNIT_t::_Day).m_dUnitDays +
            2 * cTimeUnits::GetUnitDef(TIMEUNIT_t::_Hour).m_dUnitDays +
            3 * cTimeUnits::GetUnitDef(TIMEUNIT_t::_Minute).m_dUnitDays +
            4 * cTimeUnits::GetUnitDef(TIMEUNIT_t::_Second).m_dUnitDays +
                       5 * cTimeUnits::GetUnitDef(TIMEUNIT_t::_Millisecond).m_dUnitDays +
            6 * cTimeUnits::GetUnitDef(TIMEUNIT_t::_Microsecond).m_dUnitDays;

        cString s1 = cTimeDouble::GetTimeSpanStr(dDays, TIMEUNIT_t::_Day, 6);
        UNITTEST_TRUE(!s1.Compare(_GT("1 day 2 hours 3 minutes 4.00501 seconds")));
    }
};
UNITTEST2_REGISTER(cTimeDouble, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
