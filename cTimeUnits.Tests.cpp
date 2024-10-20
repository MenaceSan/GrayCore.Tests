//! @file cTimeUnits.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cTimeUnits.h>
#include <GrayCore/include/cTimeZone.h>

namespace Gray {
struct cUnitTestTime {
    cTimeUnits m_tu;  // a test date.
    // expected results of tests.
    TIMEDOW_t m_eDOW;
    bool m_bInDST;    // is this in DST ?
    int m_iLeapDays;  // how many leap days should we use ?
    static const cUnitTestTime k_Tests[];
};

// clang-format off
const cUnitTestTime cUnitTestTime::k_Tests[] =  // static
    {
        // a sampling of test dates.
        {cTimeUnits(1970, 1, 1), TIMEDOW_t::_Thu, false, -7},  // start of cTimeInt
        {cTimeUnits(1980, 12, 1), TIMEDOW_t::_Mon, false, -5},  
        {cTimeUnits(2000, 1, 1), TIMEDOW_t::_Sat, false, 0},

        {cTimeUnits(2000, 1, 15), TIMEDOW_t::_Sat, false, 0},    
        {cTimeUnits(2001, 1, 15), TIMEDOW_t::_Mon, false, 1},     
        {cTimeUnits(2000, 2, 15), TIMEDOW_t::_Tue, false, 0},     
        {cTimeUnits(2007, 9, 15), TIMEDOW_t::_Sat, true, 2},

        {cTimeUnits(1999, 1, 15), TIMEDOW_t::_Fri, false, 0},  // 6
        {cTimeUnits(2006, 3, 15), TIMEDOW_t::_Wed, false, 2},     
        {cTimeUnits(2010, 4, 15), TIMEDOW_t::_Thu, true, 3},       
        {cTimeUnits(2020, 5, 15), TIMEDOW_t::_Fri, true, 5},

        {cTimeUnits(1999, 4, 3, 4, 1), TIMEDOW_t::_Sat, false, 0},  // 10
        {cTimeUnits(1999, 4, 29, 4, 1), TIMEDOW_t::_Thu, true, 0}, 
        {cTimeUnits(1999, 10, 3, 4, 1), TIMEDOW_t::_Sun, true, 0},
        {cTimeUnits(1999, 10, 31, 4, 1), TIMEDOW_t::_Sun, false, 0},

        {cTimeUnits(2007, 3, 7, 4, 1), TIMEDOW_t::_Wed, false, 2},  // 14
        {cTimeUnits(2007, 3, 15, 4, 1), TIMEDOW_t::_Thu, true, 2},
        {cTimeUnits(2007, 11, 1, 0, 1), TIMEDOW_t::_Thu, true, 2}, 
        {cTimeUnits(2007, 11, 8, 4, 1), TIMEDOW_t::_Thu, false, 2},
};
// clang-format on

struct UNITTEST_N(cTimeUnits) : public cUnitTest {
    UNITTEST_METHOD(cTimeUnits) {
        UNITTEST_TRUE(cTimeUnits::GetUnitDef(TIMEUNIT_t::_Day)._nUnitSeconds == (24 * 60 * 60));
        UNITTEST_TRUE(cTimeUnits::GetUnitDef(TIMEUNIT_t::_Hour)._nUnitSeconds == (60 * 60));
        UNITTEST_TRUE(cTimeUnits::GetUnitDef(TIMEUNIT_t::_Minute)._nUnitSeconds == (60));
        UNITTEST_TRUE(cTimeUnits::GetUnitDef(TIMEUNIT_t::_Second)._nUnitSeconds == 1);

        TIMEVALU_t nTimeZoneOffset = cTimeZoneMgr::GetLocalMinutesWest();
        TIMEVALU_t iTimeZoneHours = nTimeZoneOffset / 60;  // EST = -5 hours.
        UNITTEST_TRUE(iTimeZoneHours < 24);

#if 0  // def _WIN32
			int hours = 0;
			_get_daylight(&hours);
			char szTZName[100];
			size_t nTZSize;
			_get_tzname(&nTZSize, szTZName, STRMAX(szTZName), 0);
#endif

        for (size_t i = 0; i < _countof(cUnitTestTime::k_Tests); i++) {
            const cUnitTestTime& t = cUnitTestTime::k_Tests[i];
            const cTimeUnits& Tu = t.m_tu;
            TIMEDOW_t eDOW = cTimeUnits::GetDOW(Tu._wYear, Tu._wMonth, Tu._wDay);
            UNITTEST_TRUE(eDOW == t.m_eDOW);
            bool bInDST = t.m_tu.isInDST1();
            UNITTEST_TRUE(bInDST == t.m_bInDST);

            UINT32 dwDosDate1 = t.m_tu.get_DosDate();
            cTimeUnits Tu2;
            Tu2.put_DosDate(dwDosDate1);
            UNITTEST_TRUE(Tu2.get_DosDate() == dwDosDate1);  // reciprocal

            int iLeapDays = cTimeUnits::GetLeapYearsSince2K(Tu._wYear);
            UNITTEST_TRUE(iLeapDays == t.m_iLeapDays);
        }

        //**********************
        // Time spans

        cTimeUnits Tu2;
        UNITTEST_TRUE(Tu2.isValidMonth());
        UNITTEST_TRUE(Tu2.isValidTimeUnits());

        Tu2.AddSeconds((2 * cTimeUnits::k_nSecondsPerDay) + (1 * 60 * 60) + 5);  // "2 days 1 hour 5 seconds"

        GChar_t szTmp[256];
        StrLen_t iLen = Tu2.GetTimeSpanStr(TOSPAN(szTmp), TIMEUNIT_t::_Day, 2);
        UNITTEST_TRUE(iLen == 13);  // "2 days 1 hour" -> 5 seconds lost.
        UNITTEST_TRUE(StrT::Cmp<GChar_t>(szTmp, _GT("2 days 1 hour")) == 0);

        iLen = Tu2.GetTimeSpanStr(TOSPAN(szTmp), TIMEUNIT_t::_Day, 3);
        UNITTEST_TRUE(iLen == 23);  // "2 days 1 hour 5 seconds"

        Tu2 = cTimeUnits(2015, 1, 1, 12, 30, 55);                                 // need start time for month lengths.
        Tu2.AddSeconds((52 * cTimeUnits::k_nSecondsPerDay) + (1 * 60 * 60) + 5);  // "52 days 1 hour 5 seconds"
        iLen = Tu2.GetFormStr(TOSPAN(szTmp), TIMEFORMAT_t::_DEFTZ);
        UNITTEST_TRUE(iLen == 21);  //
        UNITTEST_TRUE(StrT::Cmp<GChar_t>(szTmp, _GT("2015-02-22 13:31:00 Z")) == 0);

        //**********************

        cTimeParser p1(_GT("2015/01/02T14:03:03EST"), nullptr);
        HRESULT hRes = p1.GetTimeUnits(Tu2);
        UNITTEST_TRUE(hRes == 22);
        iLen = Tu2.GetFormStr(TOSPAN(szTmp), TIMEFORMAT_t::_ISO_TZ);
        UNITTEST_TRUE(iLen == 22);  //
        UNITTEST_TRUE(StrT::Cmp<GChar_t>(szTmp, _GT("2015/01/02T14:03:03EST")) == 0);

        cTimeParser p2(_GT("2015/01/02 11:03:03.001 PM EST"), nullptr);
        hRes = p2.GetTimeUnits(Tu2);
        UNITTEST_TRUE(hRes == 30);
        iLen = Tu2.GetFormStr(TOSPAN(szTmp), TIMEFORMAT_t::_ISO_TZ);
        UNITTEST_TRUE(iLen == 22);  //
        UNITTEST_TRUE(StrT::Cmp<GChar_t>(szTmp, _GT("2015/01/02T23:03:03EST")) == 0);

        //@todo parse odd time zone storage .  (-03:00)
        // cTimeParser p3("2008-04-10T06:30:00.0000000-07:00", nullptr);
        // hRes = p3.GetTimeUnits(Tu2);
        // UNITTEST_TRUE(hRes == 30);

        // https://msdn.microsoft.com/library/az4se3k1%28v=vs.100%29.aspx?f=255&MSPPError=-2147217396&cs-save-lang=1&cs-lang=csharp#code-snippet-17

        // "04/10/2008"
        // "4/10/2008"
        // "10/04/2008"
        // "10.04.2008"
        // "Thursday, April 10, 2008"
        // "Monday, June 15, 2009 8:45:30 PM"
        // "4/10/2008 6:30 AM"
        // "04/10/2008 06:30"
        // "6/15/2009 1:45:30 PM"
        // "4/12/2008 9:30:00 AM -08:00"
        // "2009-06-15 20:45:30Z"
        // "2008-04-10T06:30:00.0000000"
        // "2008-04-10T06:30:00.0000000-07:00"
        // "Thu, 10 Apr 2008 13:30:00 GMT"
        // "Thursday, April 10, 2008 1:30:00 PM"
        // "2008-04-10T06:30:00"
        // "2015/01/02T14:03:03EST"

        // Time alone should still be parsable
        // "6:30"
        // "6:30 AM"
        // "6:30:00"
        // "6:30:00 AM"

        // ODD date formats?:
        // "April 10"
        // "10 April"
        // "April, 2008"
        // "April 2008"
    }
};
UNITTEST2_REGISTER(cTimeUnits, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
