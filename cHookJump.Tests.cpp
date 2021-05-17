// 
//! @file cHookJump.Tests.cpp
//
#include "pch.h"
#include "cHookJump.h"

namespace Gray
{
	static cHookJump sm_cHookJump_HookAPI;

	UNITTEST_CLASS(cHookJump)
	{
		// Make sure this code is not optimized out !
#ifdef _MSC_VER
#pragma optimize( "", off )
#endif
		static INT_PTR GRAYCALL HookJump1() // never inline optimize this
		{
			cUnitTests& uts = cUnitTests::I();
			uts.m_pLog->addDebugInfoF("cUnitTest_HookJump1");
			return 1;
		}
		static INT_PTR GRAYCALL HookJump2() // never inline optimize this
		{
			// replace cUnitTest_HookJump1 with this.
			cUnitTests& uts = cUnitTests::I();
			uts.m_pLog->addDebugInfoF("cUnitTest_HookJump2");
			return 2;
		}
		static void GRAYCALL HookAPI(const char* psz) // never inline optimize this
		{
			// sm_HookAPI. replace OutputDebugString() with this.

			cHookSwapLock lock(sm_cHookJump_HookAPI);
			::OutputDebugStringA("PREFIX WITH CUSTOM:");	// raw call.
			::OutputDebugStringA(psz);	// raw call.
		}

#ifdef _MSC_VER
#pragma optimize( "", on )	// restore old params.
#endif

		void TestBasic()
		{
			// Test replacing a internal call.
			INT_PTR iRet = HookJump1();	// test with no hooks.
			UNITTEST_TRUE(iRet == 1);

			cHookJump tester;
			HRESULT hRes = tester.InstallHook((FARPROC)HookJump1, (FARPROC)HookJump2);
			UNITTEST_TRUE(tester.isHookInstalled());

			iRet = HookJump1();	// really calls cUnitTest_HookJump2
			UNITTEST_TRUE(iRet == 2);

			iRet = HookJump2();
			UNITTEST_TRUE(iRet == 2);

			{
				cHookSwapLock lock(tester);
				iRet = HookJump1();	// cUnitTest_HookJump1 was restored globally.
				UNITTEST_TRUE(iRet == 1);
			}

			iRet = HookJump1();
			UNITTEST_TRUE(iRet == 2);

			{
				cHookSwapChain<> lock(tester);
				iRet = HookJump1();	// cUnitTest_HookJump1 still as cUnitTest_HookJump2.
				UNITTEST_TRUE(iRet == 2);
				iRet = lock.m_pFuncChain();	// cUnitTest_HookJump1 was restored just for Chain.
				UNITTEST_TRUE(iRet == 1);
			}

			iRet = HookJump1();	// cUnitTest_HookJump1 as cUnitTest_HookJump2.
			UNITTEST_TRUE(iRet == 2);

			tester.RemoveHook();
			UNITTEST_TRUE(!tester.isHookInstalled());

			iRet = HookJump1();
			UNITTEST_TRUE(iRet == 1);	// was restored.
			iRet = HookJump2();
			UNITTEST_TRUE(iRet == 2);		// still works as expected.
		}

		typedef VOID(FAR WINAPI* OutputDebugStringA_t)(_In_opt_ LPCSTR lpOutputString);

		void TestAPI()
		{
			// Test replacing a call i pull from a system DLL.
			// sm_HookAPI

			OutputDebugStringA_t pFuncOrig = ::OutputDebugStringA;

			::OutputDebugStringA("TestAPI() 1 (No prefix)\n");	// raw call.

			HRESULT hRes = sm_cHookJump_HookAPI.InstallHook((FARPROC)pFuncOrig, (FARPROC)HookAPI, true);

			::OutputDebugStringA("TestAPI() call\n");	// raw call.

			sm_cHookJump_HookAPI.RemoveHook();

			::OutputDebugStringA("TestAPI() 2 (No prefix)\n");	// raw call.
		}

		UNITTEST_METHOD(cHookJump)
		{
			//! hook a API function for one call.

			cUnitTests& uts = cUnitTests::I();
			UNITTEST_TRUE(uts.m_pLog != nullptr);

			TestBasic();
			TestAPI();
		}
	};
	UNITTEST2_REGISTER(cHookJump, UNITTEST_LEVEL_Core);
}
