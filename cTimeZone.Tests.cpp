//! @file cTimeZone.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cTimeZone.h>

namespace Gray {
struct UNITTEST_N(cTimeZone) : public cUnitTest {
    UNITTEST_METHOD(cTimeZone) {
        TIMEVALU_t noffset = cTimeZoneMgr::GetLocalMinutesWest();

        const cTimeZone* pTz1 = cTimeZoneMgr::FindTimeZone(TZ_EST);
        UNITTEST_TRUE(pTz1 != nullptr);
    }
};
UNITTEST2_REGISTER(cTimeZone, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
