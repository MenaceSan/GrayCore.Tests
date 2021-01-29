//
//! @file GrayCore.Tests.cpp
//! Support M$ testing framework.

#include "pch.h"

namespace Gray
{

#ifdef USE_UNITTESTS_MS
	// NOTE: This requires runtime path set to find CppUnitTestFramework.dll or CppUnitTestFramework.x64.dll as well. 
	// $(DevEnvDir)\CommonExtensions\Microsoft\TestWindow\Extensions\CppUnitFramework = D:\Programs\Microsoft Visual Studio 12.0\Common7\IDE\CommonExtensions\Microsoft\TestWindow\Extensions\CppUnitFramework
#pragma comment(lib,"Microsoft.VisualStudio.TestTools.CppUnitTestFramework.lib")

	class cUnitTestsMsLogger : public cLogAppender, public cRefBase
	{
		IUNKNOWN_DISAMBIG(cRefBase);
		virtual HRESULT WriteString(const LOGCHAR_t* pszMsg) override
		{
			//! Pass messages to M$
			::Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(pszMsg);
			return 1;
		}
	};

	bool CALLBACK UnitTestsMs_AssertCallback(const char* pszExp, const cDebugSourceLine& src) // static AssertCallback_t
	{
		//! Assert was called during a unit test. This is a failure!
		//! AssertCallback_t sm_pAssertCallback = cDebugAssert::Assert_Fail was called in UnitTest. GRAYCALL

		__LineInfo lineInfo(cStringW(src.m_pszFile), src.m_pszFunction, src.m_uLine);	// StrArg
		Assert::IsTrue(false, cStringW(pszExp), &lineInfo);
		return false;
	}

	TEST_MODULE_INITIALIZE(Initialize)
	{
		// module initialization code

		cUnitTests& uts = cUnitTests::I();
		uts.m_pAssertOrig = cDebugAssert::sm_pAssertCallback;
		cDebugAssert::sm_pAssertCallback = UnitTestsMs_AssertCallback;		// special version of assert

		// add special log appender. Logger. cUnitTestLogger
		cLogMgr& log = cLogMgr::I();
		if (log.FindAppenderType(typeid(cUnitTestsMsLogger), true) == nullptr)
		{
			log.AddAppender(new cUnitTestsMsLogger());	// route logs here.
		}
	}
	TEST_MODULE_CLEANUP(Cleanup)
	{
		// module cleanup code
		cUnitTests& uts = cUnitTests::I();
		cLogMgr& log = cLogMgr::I();
		log.RemoveAppenderType(typeid(cUnitTestsMsLogger), true);
		cDebugAssert::sm_pAssertCallback = uts.m_pAssertOrig;	// restore.
	}

#endif

	UNITTEST2_CLASS(GrayCore)
	{
		// https://docs.microsoft.com/en-us/visualstudio/test/writing-unit-tests-for-c-cpp?view=vs-2019

	public:

		UNITTEST2_METHOD(GrayCore)
		{
			static_assert(!USE_UNICODE_FN, "USE_UNICODE_FN");
			UNITTEST2_TRUE(true);

			cUnitTests& uts = cUnitTests::I();
			// cUnitTests::AssertValid
		}
	};

	UNITTEST2_REGISTER(GrayCore, UNITTEST_LEVEL_Core);
}
