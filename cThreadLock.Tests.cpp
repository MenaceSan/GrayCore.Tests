//
//! @file cThreadLock.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cThreadLock.h>

namespace Gray {
struct UNITTEST_N(cThreadLock) : public cUnitTest {
    UNITTEST_METHOD(cThreadLock) {
        cUnitTests& uts = cUnitTests::I();

        // NOTE: See cThread UnitTest for better testing of locks.
        const THREADID_t tidCurrent = cThreadId::GetCurrentId();
        UNITTEST_TRUE(cThreadId::IsValidId(tidCurrent));

        cThreadLockFast lockFast;
        UNITTEST_TRUE(lockFast.isIdle());
        {
            const auto guard(lockFast.Lock());
            UNITTEST_TRUE(lockFast.isLocked());
        }
        UNITTEST_TRUE(lockFast.isIdle());

        cThreadLockCrit lockCrit;
        UNITTEST_TRUE(lockCrit.isIdle());
        {
            const auto guard(lockCrit.Lock());
            UNITTEST_TRUE(lockCrit.isLocked());
        }
        UNITTEST_TRUE(lockCrit.isIdle());

        cThreadLockMutex lockMutex;
        UNITTEST_TRUE(lockMutex.isIdle());
        {
            const auto guard(lockMutex.Lock());
            UNITTEST_TRUE(lockMutex.isLocked());
        }
        UNITTEST_TRUE(lockMutex.isIdle());
    }
};
UNITTEST2_REGISTER(cThreadLock, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
