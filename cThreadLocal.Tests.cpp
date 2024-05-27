//! @file cThreadLocal.Tests.cpp
#include "pch.h"
#include <GrayCore/include/cThreadLocalSys.h>
#include <GrayCore/include/cString.h>

namespace Gray {
struct UNITTEST_N(cThreadLocal) : public cUnitTest {
    UNITTEST_METHOD(cThreadLocal) {
        cUnitTests& uts = cUnitTests::I();
        cString s1 = "werwer";
        cString s2 = s1;
        cThreadLocalSysT<cString> thread1;
        UNITTEST_TRUE(thread1.GetData() == nullptr);
        thread1.PutData(&s1);
        UNITTEST_TRUE(thread1.GetData() == &s1);
        thread1.PutData(nullptr);
        UNITTEST_TRUE(thread1.GetData() == nullptr);
    }
};
UNITTEST2_REGISTER(cThreadLocal, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
