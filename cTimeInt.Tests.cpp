//! @file cTimeInt.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cFile.h>
#include <GrayCore/include/cFileDir.h>
#include <GrayCore/include/cMime.h>
#include <GrayCore/include/cTimeInt.h>

namespace Gray {
struct UNITTEST_N(cTimeInt) : public cUnitTest {
    void UnitTest2(const GChar_t* pszDateTime, TZ_TYPE nTimeZone) {
        // @arg nTimeZone = TZ_UTC,TZ_GMT

        cUnitTests& uts = cUnitTests::I();

        cTimeInt testtime1;
        HRESULT hRes = testtime1.SetTimeStr(pszDateTime, nTimeZone);
        UNITTEST_TRUE(hRes > 0);

        // Test read and write of a variety of time formats.
        for (ITERATE_t i = 0; i < static_cast<int>(TIMEFORMAT_t::_QTY); i++) {
            cString sTimeTest1 = testtime1.GetTimeFormStr((TIMEFORMAT_t)i, nTimeZone);

            if (i == static_cast<int>(TIMEFORMAT_t::_ASN)) continue;  // we don't read this type.                

            cTimeInt testtime2;
            hRes = testtime2.SetTimeStr(sTimeTest1, nTimeZone);
            UNITTEST_TRUE(hRes > 0);
            UNITTEST_TRUE(testtime1 == testtime2);

            cString sTimeTest2 = testtime2.GetTimeFormStr((TIMEFORMAT_t)i, nTimeZone);
            UNITTEST_TRUE(sTimeTest1.IsEqualNoCase(sTimeTest2));
        }

        cStringF sFilePath = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("TimeUnitTest") _FN(MIME_EXT_txt));

        // Get and set the change times on a file ? FILE_WRITE_ATTRIBUTES
        cFile file;
        hRes = file.OpenX(sFilePath, OF_CREATE | OF_WRITE | OF_BINARY);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        hRes = file.SetFileTime(testtime1, testtime1);
        if (hRes <= 0) {
            UNITTEST_TRUE(hRes > 0);
            hRes = HResult::GetLastDef(E_HANDLE);
            return;
        }
        cFileStatus filestatus;
        hRes = filestatus.ReadFileStatus(sFilePath);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        UNITTEST_TRUE(filestatus.IsSameChangeTime(testtime1));
    }

    UNITTEST_METHOD(cTimeInt) {
        // ASSUME _tzset() has been called.
        cUnitTests& uts = cUnitTests::I();

#if 0  // def _DEBUG
			TIMESEC_t tmaxu = (UINT_MAX);
			TIMESEC_t tmaxs = (INT_MAX);
			UNITTEST_TRUE(tmaxs > tmaxu);	// assumed to be signed
#endif

        cTimeInt tnow1;
        tnow1.InitTimeNow();
        UNITTEST_TRUE(tnow1.isTimeValid());
        cTimeUnits tn;
        UNITTEST_TRUE(tnow1.GetTimeUnits(tn, TZ_LOCAL));
        UNITTEST_TRUE(tn.isReasonableTimeUnits());

        uts.m_pLog->addDebugInfoF("Current local time is '%s'", LOGSTR(tnow1.GetTimeFormStr(TIMEFORMAT_t::_DEFTZ, TZ_LOCAL)));

        int iLeapYears = cTimeUnits::GetLeapYearsSince2K(1970);
        UNITTEST_TRUE(iLeapYears == -7);

        cTimeInt y2k;
        HRESULT hRes = y2k.SetTimeStr(_GT("2000/1/1 00:00:00"), TZ_UTC);
        UNITTEST_TRUE(hRes > 0);

        TIMESEC_t t = y2k.GetTime();
        UNITTEST_TRUE(t == cTimeInt::k_nY2K);
        hRes = y2k.SetTimeStr(_GT("2000/1/1 01:01:01"), TZ_UTC);
        UNITTEST_TRUE(hRes > 0);

        UNITTEST_TRUE(y2k.GetTime() == cTimeInt::k_nY2K + 60 * 60 + 60 + 1);

        UnitTest2(_GT("2004/8/7 01:20:20"), TZ_LOCAL);  // Daylight savings time
        UnitTest2(_GT("2004/1/14 01:20:20"), TZ_UTC);   // Standard time
        UnitTest2(_GT("2006/5/14 01:20:20 G"), TZ_UTC);
        UnitTest2(_GT("2005/8/20 20:45:10 G"), TZ_LOCAL);

        // test spans.

        TIMESECD_t uVal1 = 1 * (24 * 60 * 60) + 2 * (60 * 60) + 3 * 60 + 4;

        cString s1 = cTimeInt::GetTimeSpanStr(uVal1, TIMEUNIT_t::_Day, 4);
        UNITTEST_TRUE(s1.IsEqualNoCase(_GT("1 day 2 hours 3 minutes 4 seconds")));
        cString s2 = cTimeInt::GetTimeDeltaBriefStr(uVal1);
        UNITTEST_TRUE(s2.IsEqualNoCase(_GT("1d 2h 3m 4s")));
        cString s3 = cTimeInt::GetTimeDeltaSecondsStr(uVal1);
        UNITTEST_TRUE(s3.IsEqualNoCase(_GT("26 hours 3 minutes 4 seconds")));
    }
};
UNITTEST2_REGISTER(cTimeInt, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
