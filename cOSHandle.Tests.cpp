//! @file cOSHandle.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cOSHandle.h>

namespace Gray {
struct UNITTEST_N(cOSHandle) : public cUnitTest {
    UNITTEST_METHOD(cOSHandle) {
        cOSHandle h1;
        cOSHandle h2;
#ifdef _WIN32
        // Test dupe of OS handle.
#endif
    }
};
UNITTEST2_REGISTER(cOSHandle, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
