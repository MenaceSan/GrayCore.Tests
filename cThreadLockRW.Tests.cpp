//
//! @file cThreadLockRW.Tests.cpp
//
#include "pch.h"
#include "cThreadLockRW.h"
#include "cThreadLocalSys.h"

namespace Gray
{
	UNITTEST_CLASS(cThreadLockRW)
	{
		UNITTEST_METHOD(cThreadLockRW)
		{
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
	UNITTEST2_REGISTER(cThreadLockRW, UNITTEST_LEVEL_Core);
}
