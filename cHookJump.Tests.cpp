// 
//! @file cHookJump.Tests.cpp
//
#include "pch.h"
#include "cHookJump.h"

namespace Gray
{
	// Make sure this code is not optimized out !
#ifdef _MSC_VER
#pragma optimize( "", off )
#endif
	INT_PTR GRAYCALL cUnitTest_HookJump1() // never inline optimize this
	{
		cUnitTests& uts = cUnitTests::I();
		uts.m_pLog->addDebugInfoF("cUnitTest_HookJump1");
		return 1;
	}
	INT_PTR GRAYCALL cUnitTest_HookJump2() // never inline optimize this
	{
		// replace cUnitTest_HookJump1 with this.
		cUnitTests& uts = cUnitTests::I();
		uts.m_pLog->addDebugInfoF("cUnitTest_HookJump2");
		return 2;
	}
#ifdef _MSC_VER
#pragma optimize( "", on )	// restore old params.
#endif

	UNITTEST2_CLASS(cHookJump)
	{
		UNITTEST2_METHOD(cHookJump)
		{
			//! hook a API function for one call.

			cUnitTests& uts = cUnitTests::I();
			UNITTEST2_TRUE(uts.m_pLog != nullptr);

			INT_PTR iRet = cUnitTest_HookJump1();
			UNITTEST2_TRUE(iRet == 1);

			cHookJump tester;
			tester.InstallHook((FARPROC)cUnitTest_HookJump1, (FARPROC)cUnitTest_HookJump2);
			UNITTEST2_TRUE(tester.isHookInstalled());

			iRet = cUnitTest_HookJump1();	// really calls cUnitTest_HookJump2
			UNITTEST2_TRUE(iRet == 2);

			iRet = cUnitTest_HookJump2();
			UNITTEST2_TRUE(iRet == 2);

			{
				cHookSwapLock lock(tester);
				iRet = cUnitTest_HookJump1();	// cUnitTest_HookJump1 was restored globally.
				UNITTEST2_TRUE(iRet == 1);
			}

			iRet = cUnitTest_HookJump1();
			UNITTEST2_TRUE(iRet == 2);

			{
				cHookSwapChain lock(tester);
				iRet = cUnitTest_HookJump1();	// cUnitTest_HookJump1 still as cUnitTest_HookJump2.
				UNITTEST2_TRUE(iRet == 2);
				iRet = lock.m_pFuncChain();	// cUnitTest_HookJump1 was restored just for Chain.
				UNITTEST2_TRUE(iRet == 1);
			}

			iRet = cUnitTest_HookJump1();	// cUnitTest_HookJump1 as cUnitTest_HookJump2.
			UNITTEST2_TRUE(iRet == 2);

			tester.RemoveHook();
			UNITTEST2_TRUE(!tester.isHookInstalled());

			iRet = cUnitTest_HookJump1();
			UNITTEST2_TRUE(iRet == 1);	// was restored.
			iRet = cUnitTest_HookJump2();
			UNITTEST2_TRUE(iRet == 2);		// still works as expected.
		}
	};
	UNITTEST2_REGISTER(cHookJump, UNITTEST_LEVEL_Core);
}
