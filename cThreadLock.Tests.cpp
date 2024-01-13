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
        THREADID_t idCurrent = cThreadId::GetCurrentId();
        UNITTEST_TRUE(cThreadId::IsValidId(idCurrent));

        cThreadLockFast lockFast;
        lockFast.Lock();
        lockFast.Unlock();

        cThreadLockCrit lockCrit;
        lockCrit.Lock();
        lockCrit.Unlock();

        cThreadLockMutex lockMutex;
        lockMutex.Lock();
        lockMutex.Unlock();

        // cThreadLockRef
    }
};
UNITTEST2_REGISTER(cThreadLock, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
