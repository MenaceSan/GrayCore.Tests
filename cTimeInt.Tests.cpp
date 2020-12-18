// 
//! @file cTimeInt.Tests.cpp
//
#include "pch.h"
#include "cTimeInt.h"
#include "cFile.h"
#include "cFileDir.h"
#include "cMime.h"

namespace Gray
{
	UNITTEST2_CLASS(cTimeInt)
	{
		void UnitTest2(const GChar_t * pszDateTime, TZ_TYPE nTimeZoneOffset)
		{
			// @arg nTimeZoneOffset = TZ_UTC,TZ_GMT

			cUnitTests& uts = cUnitTests::I();

			cTimeInt testtime1;
			HRESULT hRes = testtime1.SetTimeStr(pszDateTime, nTimeZoneOffset);
			UNITTEST2_TRUE(hRes > 0);

			// Test read and write of a variety of time formats.
			for (size_t i = 0; i < TIME_FORMAT_QTY; i++)
			{
				cString sTimeTest1 = testtime1.GetTimeFormStr((TIME_FORMAT_TYPE)i, nTimeZoneOffset);

				if (i == TIME_FORMAT_ASN)	// we don't read this type.
					continue;

				cTimeInt testtime2;
				hRes = testtime2.SetTimeStr(sTimeTest1, nTimeZoneOffset);
				UNITTEST2_TRUE(hRes > 0);
				UNITTEST2_TRUE(testtime1 == testtime2);

				cString sTimeTest2 = testtime2.GetTimeFormStr((TIME_FORMAT_TYPE)i, nTimeZoneOffset);
				UNITTEST2_TRUE(!sTimeTest1.CompareNoCase(sTimeTest2));
			}

			cStringF sFilePath = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("TimeUnitTest") _FN(MIME_EXT_txt));

			// Get and set the change times on a file ? FILE_WRITE_ATTRIBUTES
			cFile file;
			hRes = file.OpenX(sFilePath, OF_CREATE | OF_WRITE | OF_BINARY);
			UNITTEST2_TRUE(SUCCEEDED(hRes));
			hRes = file.SetFileTime(testtime1, testtime1);
			if (hRes <= 0)
			{
				UNITTEST2_TRUE(hRes > 0);
				hRes = HResult::GetLastDef(E_HANDLE);
				return;
			}
			cFileStatus filestatus;
			hRes = filestatus.ReadFileStatus(sFilePath);
			UNITTEST2_TRUE(SUCCEEDED(hRes));
			UNITTEST2_TRUE(filestatus.IsSameChangeTime(testtime1));
		}

		UNITTEST2_METHOD(cTimeInt)
		{
			// ASSUME _tzset() has been called.
			cUnitTests& uts = cUnitTests::I();

#if 0 // def _DEBUG
			TIMESEC_t tmaxu = (UINT_MAX);
			TIMESEC_t tmaxs = (INT_MAX);
			UNITTEST2_TRUE(tmaxs > tmaxu);	// assumed to be signed
#endif

			cTimeInt tnow1;
			tnow1.InitTimeNow();
			UNITTEST2_TRUE(tnow1.isTimeValid());
			cTimeUnits tn;
			UNITTEST2_TRUE(tnow1.GetTimeUnits(tn, TZ_LOCAL));
			UNITTEST2_TRUE(tn.isReasonableTimeUnits());

			uts.m_pLog->addDebugInfoF("Current local time is '%s'", LOGSTR(tnow1.GetTimeFormStr(TIME_FORMAT_TZ, TZ_LOCAL)));

			int iLeapYears = cTimeUnits::GetLeapYearsSince2K(1970);
			UNITTEST2_TRUE(iLeapYears == -7);

			cTimeInt y2k;
			HRESULT hRes = y2k.SetTimeStr(_GT("2000/1/1 00:00:00"), TZ_UTC);
			UNITTEST2_TRUE(hRes > 0);

			TIMESEC_t t = y2k.GetTime();
			UNITTEST2_TRUE(t == cTimeInt::k_nY2K);
			hRes = y2k.SetTimeStr(_GT("2000/1/1 01:01:01"), TZ_UTC);
			UNITTEST2_TRUE(hRes > 0);

			UNITTEST2_TRUE(y2k.GetTime() == cTimeInt::k_nY2K + 60 * 60 + 60 + 1);

			UnitTest2(_GT("2004/8/7 01:20:20"), TZ_LOCAL);	// Daylight savings time
			UnitTest2(_GT("2004/1/14 01:20:20"), TZ_UTC);	// Standard time
			UnitTest2(_GT("2006/5/14 01:20:20 G"), TZ_UTC);
			UnitTest2(_GT("2005/8/20 20:45:10 G"), TZ_LOCAL);

			// test spans.

			TIMESECD_t uVal1 = 1 * (24 * 60 * 60) + 2 * (60 * 60) + 3 * 60 + 4;

			cString s1 = cTimeInt::GetTimeSpanStr(uVal1, TIMEUNIT_Day, 4);
			UNITTEST2_TRUE(!s1.CompareNoCase(_GT("1 day 2 hours 3 minutes 4 seconds")));
			cString s2 = cTimeInt::GetTimeDeltaBriefStr(uVal1);
			UNITTEST2_TRUE(!s2.CompareNoCase(_GT("1d 2h 3m 4s")));
			cString s3 = cTimeInt::GetTimeDeltaSecondsStr(uVal1);
			UNITTEST2_TRUE(!s3.CompareNoCase(_GT("26 hours 3 minutes 4 seconds")));
		}
	};
	UNITTEST2_REGISTER(cTimeInt, UNITTEST_LEVEL_Core);
}
