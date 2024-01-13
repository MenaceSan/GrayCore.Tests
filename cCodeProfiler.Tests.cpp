//
//! @file cCodeProfiler.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cCodeProfiler.h>
#include <GrayCore/include/cThreadLock.h>

namespace Gray {
void TestFunction() {
    cCodeProfileFunc tester_cCodeProfileFunc(DEBUGSOURCELINE);
    cThreadId::SleepCurrent(2);  // Waste some time.
}

struct UNITTEST_N(cCodeProfiler) : public cUnitTest {
    UNITTEST_METHOD(cCodeProfiler) {
        TestFunction();
        // Now look to see if the function recorded something.
    }
};
UNITTEST2_REGISTER(cCodeProfiler, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
