//! @file cOSProcess.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cOSProcess.h>

namespace Gray {
struct UNITTEST_N(cOSProcess) : public cUnitTest {
    UNITTEST_METHOD(cOSProcess) {
        // get my process name.
        cOSProcess procMe;
        procMe.AttachCurrentProcess();

        cStringF sMyName = procMe.get_ProcessName();
        UNITTEST_TRUE(sMyName.GetLength() > 0);  // testhost.exe for M$ test

        // open a process for URL view ?
    }
};
UNITTEST2_REGISTER(cOSProcess, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
