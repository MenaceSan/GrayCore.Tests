//! @file cSingleton.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cSingleton.h>

namespace Gray {
struct cUnitTestSing final : public cSingleton<cUnitTestSing> {
    SINGLETON_IMPL(cUnitTestSing);
    cUnitTestSing() : cSingleton<cUnitTestSing>(this, typeid(cUnitTestSing)) {}
};

struct UNITTEST_N(cSingleton) : public cUnitTest {
    UNITTEST_METHOD(cSingleton) {
        UNITTEST_TRUE(!cUnitTestSing::isSingleCreated());
        {
            cUnitTestSing inst;
            UNITTEST_TRUE(cUnitTestSing::isSingleCreated());
            // Will be destructed at app close.
        }
        UNITTEST_TRUE(!cUnitTestSing::isSingleCreated());
    }
};
UNITTEST2_REGISTER(cSingleton, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
