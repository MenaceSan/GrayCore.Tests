//
//! @file cThreadLock.Tests.cpp
//
#include "pch.h"
#include "cThreadLock.h"

namespace Gray
{
	UNITTEST2_CLASS(cThreadLock)
	{
		UNITTEST2_METHOD(cThreadLock)
		{
			cUnitTests& uts = cUnitTests::I();

			// NOTE: See cThread UnitTest for better testing of locks.
			THREADID_t idCurrent = cThreadId::GetCurrentId();
			UNITTEST2_TRUE(cThreadId::IsValidId(idCurrent));

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

	UNITTEST2_REGISTER(cThreadLock, UNITTEST_LEVEL_Core);
}
