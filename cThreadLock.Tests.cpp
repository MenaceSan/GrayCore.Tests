//! @file cThreadLock.Tests.cpp
#include "pch.h"
#include <GrayCore/include/cThreadLock.h>

namespace Gray {
struct UNITTEST_N(cThreadLock) : public cUnitTest {
    UNITTEST_METHOD(cThreadLock) {
        cUnitTests& uts = cUnitTests::I();

        // NOTE: See cThread UnitTest for better testing of locks.
        const THREADID_t tidCurrent = cThreadId::GetCurrentId();
        UNITTEST_TRUE(cThreadId::IsValidId(tidCurrent));
        size_t sizeGuard = 0;

        cThreadLockableFast lockFast;
        const size_t sizeLockableF = sizeof(lockFast);
        UNITTEST_TRUE(sizeLockableF >= 4);
        UNITTEST_TRUE(lockFast.isIdle());
        {
            const auto guard(lockFast.Lock());
            sizeGuard = sizeof(guard);
            UNITTEST_TRUE(lockFast.isLocked());
        }
        UNITTEST_TRUE(lockFast.isIdle());

        cThreadLockableCrit lockCrit;
        const size_t sizeLockableC = sizeof(lockCrit);
        UNITTEST_TRUE(lockCrit.isIdle());
        {
            const auto guard(lockCrit.Lock());
            sizeGuard = sizeof(guard);
            UNITTEST_TRUE(lockCrit.isLocked());
        }
        UNITTEST_TRUE(lockCrit.isIdle());

        cThreadLockableMutex lockMutex;
        const size_t sizeLockableM = sizeof(lockMutex);
        UNITTEST_TRUE(lockMutex.isIdle());
        {
            const auto guard(lockMutex.Lock());
            sizeGuard = sizeof(guard);
            UNITTEST_TRUE(lockMutex.isLocked());
        }
        UNITTEST_TRUE(lockMutex.isIdle());
    }
};
UNITTEST2_REGISTER(cThreadLock, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
