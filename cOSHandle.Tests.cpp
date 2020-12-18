// 
//! @file cOSHandle.Tests.cpp
//
#include "pch.h"
#include "cOSHandle.h"

namespace Gray
{
	UNITTEST2_CLASS(cOSHandle)
	{
		UNITTEST2_METHOD(cOSHandle)
		{
			cOSHandle h1;
			cOSHandle h2;
#ifdef _WIN32
			// Test dupe of OS handle.
#endif
		}
	};
	UNITTEST2_REGISTER(cOSHandle, UNITTEST_LEVEL_Core);
}
