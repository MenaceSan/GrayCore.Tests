//
//! @file cTimeFile.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cTimeFile.h>

namespace Gray {
struct UNITTEST_N(cTimeFile) : public cUnitTest {
    UNITTEST_METHOD(cTimeFile) {
        UINT64 uVal = 0xFFFFFFFFFFFFFFFFULL;
        uVal /= cTimeFile::k_nFreq;  // seconds.
        uVal /= 60 * 60 * 24;        // days.
        uVal /= 365;                 // years

        UNITTEST_TRUE(uVal == 58494);
    }
};
UNITTEST2_REGISTER(cTimeFile, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
