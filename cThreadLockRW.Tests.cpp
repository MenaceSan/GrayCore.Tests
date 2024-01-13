//
//! @file cThreadLockRW.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cThreadLocalSys.h>
#include <GrayCore/include/cThreadLockRW.h>

namespace Gray {
struct UNITTEST_N(cThreadLockRW) : public cUnitTest {
    UNITTEST_METHOD(cThreadLockRW) {
        //! TODO cThreadLockRW
        //! Test reentrant and upgrade features.

        cThreadLockRW lockRW;
#if 0
			lockRW.IncReadLockCount();
			lockRW.Lock();

			{
			}

			lockRW.Unlock();
			lockRW.DecReadLockCount();
#endif
    }
};
UNITTEST2_REGISTER(cThreadLockRW, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
