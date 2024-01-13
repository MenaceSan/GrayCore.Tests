//
//! @file GrayCore.Tests.cpp
//! Support M$ testing framework.

#include "pch.h"
#include <GrayCore/include/cLogEvent.h>

namespace Gray {

#ifdef USE_UNITTESTS_MS
// NOTE: This requires runtime path set to find CppUnitTestFramework.dll or CppUnitTestFramework.x64.dll as well.
// $(DevEnvDir)\CommonExtensions\Microsoft\TestWindow\Extensions\CppUnitFramework = D:\Programs\Microsoft Visual Studio 12.0\Common7\IDE\CommonExtensions\Microsoft\TestWindow\Extensions\CppUnitFramework
// C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\Extensions\TestPlatform\Extensions\Cpp\Microsoft.VisualStudio.TestTools.CppUnitTestFramework.x64.dll
#pragma comment(lib, "Microsoft.VisualStudio.TestTools.CppUnitTestFramework.lib")

struct cUnitTestsMsLogger : public cLogSink, public cRefBase {
    IUNKNOWN_DISAMBIG(cRefBase);
    HRESULT addEvent(cLogEvent& rEvent) noexcept override {
        //! Pass messages to M$ unit test output
        ::Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(rEvent.get_FormattedDefault());
        return 1;
    }
};

bool CALLBACK UnitTestsMs_AssertCallback(const char* pszExp, const cDebugSourceLine& src) {  // static AssertCallback_t
    //! Assert was called during a unit test. This is a failure!
    //! AssertCallback_t sm_pAssertCallback = cDebugAssert::Assert_Fail was called in UnitTest. GRAYCALL

    __LineInfo lineInfo(cStringW(src.m_pszFile), src.m_pszFunction, src.m_uLine);  // StrArg
    Assert::IsTrue(false, cStringW(pszExp), &lineInfo);
    return false;
}

TEST_MODULE_INITIALIZE(Initialize) {
    // module initialization code
    cUnitTests& uts = cUnitTests::I();
    uts.m_pAssertOrig = cDebugAssert::sm_pAssertCallback;
    cDebugAssert::sm_pAssertCallback = UnitTestsMs_AssertCallback;  // special version of assert

    // add special log sink. Logger. cUnitTestLogger
    cLogMgr& log = cLogMgr::I();
    if (log.FindSinkType(typeid(cUnitTestsMsLogger), true) == nullptr) {
        log.AddSink(new cUnitTestsMsLogger());  // route logs here.
    }
}
TEST_MODULE_CLEANUP(Cleanup) {
    // module cleanup code
    cUnitTests& uts = cUnitTests::I();
    cLogMgr& log = cLogMgr::I();
    log.RemoveSinkType(typeid(cUnitTestsMsLogger), true);
    cDebugAssert::sm_pAssertCallback = uts.m_pAssertOrig;  // restore.
}
#endif

struct UNITTEST_N(GrayCore) : public cUnitTest {  // https://docs.microsoft.com/en-us/visualstudio/test/writing-unit-tests-for-c-cpp?view=vs-2019
    UNITTEST_METHOD(GrayCore) {
        UNITTEST_TRUE(true);
        cUnitTests& uts = cUnitTests::I();
        // cUnitTests::AssertValid
    }  // namespace Gray
};

UNITTEST2_REGISTER(GrayCore, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
