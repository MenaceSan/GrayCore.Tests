//
//! @file cSystemInfo.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cSystemHelper.h>
#include <GrayCore/include/cSystemInfo.h>

namespace Gray {
struct UNITTEST_N(cSystemInfo) : public cUnitTest {
    UNITTEST_METHOD(cSystemInfo) {
        cSystemHelper& i = cSystemHelper::I();

        cStringF sSysName = i.get_SystemName();
        UNITTEST_TRUE(!sSysName.IsEmpty());

        cString sOsName = i.get_OSInfoStr();
        UNITTEST_TRUE(!sOsName.IsEmpty());

        UINT uOSVer = i.m_Info.get_OSVer();
#ifdef _WIN32
        UNITTEST_TRUE(uOSVer > 0x500);  // For windows.
#endif
#ifdef __linux__
        UNITTEST_TRUE(uOSVer > 0);  // For __linux__.
#endif
        cSystemInfo::SystemBeep();
    }
};
UNITTEST2_REGISTER(cSystemInfo, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
