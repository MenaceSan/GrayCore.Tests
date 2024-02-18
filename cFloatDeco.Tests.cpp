//
//! @file cFloatDeco.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cFloatDeco.h>

namespace Gray {
struct UNITTEST_N(cFloatDeco) : public cUnitTest {
    UNITTEST_METHOD(cFloatDeco) {
        int nExp10 = 0;
        cFloatDeco v1 = cFloatDeco::GetCachedPower(801, nExp10);
        UNITTEST_TRUE(v1.m_uMant == 16905424996341287883);
        UNITTEST_TRUE(v1.m_iExp2 == -901);
        UNITTEST_TRUE(nExp10 == 252);
    }
};
UNITTEST2_REGISTER(cFloatDeco, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
