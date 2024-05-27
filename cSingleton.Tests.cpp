//! @file cSingleton.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cSingleton.h>

namespace Gray {
struct cUnitTestSingStat1 final : public cSingletonStatic<cUnitTestSingStat1> {
    cUnitTestSingStat1() : cSingletonStatic<cUnitTestSingStat1>(this ) {
        DEBUG_MSG(("cUnitTestSingStat1"));
    }
    ~cUnitTestSingStat1() override {
        DEBUG_MSG(("~cUnitTestSingStat1")); // will close in cAppState::isInCExit()
    }
    using cSingletonStatic::ClearSing;
};
struct cUnitTestSingStat2 final : public cSingletonStatic<cUnitTestSingStat2> {
    cUnitTestSingStat2() : cSingletonStatic<cUnitTestSingStat2>(this) {
        DEBUG_MSG(("cUnitTestSingStat2"));
    }
    ~cUnitTestSingStat2() override {
        DEBUG_MSG(("~cUnitTestSingStat2"));
    }
};
class cUnitTestSing final : public cSingleton<cUnitTestSing> {
    SINGLETON_IMPL(cUnitTestSing);

 protected:
    cUnitTestSing() : cSingleton<cUnitTestSing>(this, typeid(cUnitTestSing)) {
        DEBUG_MSG(("cUnitTestSing"));
    }

 public:
    ~cUnitTestSing() override {
        DEBUG_MSG(("~cUnitTestSing"));
    }
};

struct UNITTEST_N(cSingleton) : public cUnitTest {
    UNITTEST_METHOD(cSingleton) {

        // static will only run once!
        if (!cUnitTestSingStat1::isSingleCreated()) {  // only true on first run of test.
            static cUnitTestSingStat1 localStatic;
            UNITTEST_TRUE(cUnitTestSingStat1::isSingleCreated());
        }

        // local
        UNITTEST_TRUE(!cUnitTestSingStat2::isSingleCreated());
        {
            cUnitTestSingStat2 singStat;
            UNITTEST_TRUE(cUnitTestSingStat2::isSingleCreated());
        }
        UNITTEST_TRUE(!cUnitTestSingStat2::isSingleCreated());

        // dynamic.
        if (cUnitTestSing::isSingleCreated()) {
            cUnitTestSing& testRef = cUnitTestSing::I();
            UNITTEST_TRUE(cUnitTestSing::isSingleCreated());
            // Will be destructed at app close.
        }
    }
};
UNITTEST2_REGISTER(cSingleton, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
