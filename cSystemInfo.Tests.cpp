// 
//! @file cSystemInfo.Tests.cpp
//
#include "pch.h"
#include "cSystemInfo.h"
#include "cSystemHelper.h"

namespace Gray
{
	UNITTEST2_CLASS(cSystemInfo)
	{
		UNITTEST2_METHOD(cSystemInfo)
		{
			cSystemHelper& i = cSystemHelper::I();

			cStringF sSysName = i.get_SystemName();
			UNITTEST2_TRUE(!sSysName.IsEmpty());

			cString sOsName = i.get_OSInfoStr();
			UNITTEST2_TRUE(!sOsName.IsEmpty());

			UINT uOSVer = i.m_Info.get_OSVer();
#ifdef _WIN32
			UNITTEST2_TRUE(uOSVer > 0x500);	// For windows.
#endif
#ifdef __linux__
			UNITTEST2_TRUE(uOSVer > 0);	// For __linux__.
#endif
			cSystemInfo::SystemBeep();
		}
	};
	UNITTEST2_REGISTER(cSystemInfo, UNITTEST_LEVEL_Core);
}
