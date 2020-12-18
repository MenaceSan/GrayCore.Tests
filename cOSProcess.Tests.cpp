// 
//! @file cOSProcess.Tests.cpp
//
#include "pch.h"
#include "cOSProcess.h"

namespace Gray
{
	UNITTEST2_CLASS(cOSProcess)
	{
		UNITTEST2_METHOD(cOSProcess)
		{
			// get my process name.
			cOSProcess procMe;
			procMe.AttachCurrentProcess();

			cStringF sMyName = procMe.get_ProcessName();
			UNITTEST2_TRUE(sMyName.GetLength() > 0);		// testhost.exe for M$ test

			// open a process for URL view ?
		}
	};
	UNITTEST2_REGISTER(cOSProcess, UNITTEST_LEVEL_Core);
}

