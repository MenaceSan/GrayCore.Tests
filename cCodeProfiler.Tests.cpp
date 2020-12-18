//
//! @file cCodeProfiler.Tests.cpp
//
#include "pch.h"
#include "cCodeProfiler.h"
#include "cThreadLock.h"

namespace Gray
{
	void TestFunction()
	{
		cCodeProfileFunc tester_cCodeProfileFunc(DEBUGSOURCELINE);
		cThreadId::SleepCurrent(2);		// Waste some time.
	}

	UNITTEST2_CLASS(cCodeProfiler)
	{
		UNITTEST2_METHOD(cCodeProfiler)
		{
			TestFunction();
			// Now look to see if the function recorded something.
		}
	};
	UNITTEST2_REGISTER(cCodeProfiler, UNITTEST_LEVEL_Core);
}
