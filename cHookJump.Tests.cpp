//! @file cHookJump.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cHookJump.h>

namespace Gray {
// typedef VOID(FAR WINAPI* OutputDebugStringA_t)(_In_opt_ LPCSTR lpOutputString);
static int sm_Hook_Calls = 0;
static cHookJumpT<decltype(&::OutputDebugStringA)> sm_cHookJump_HookAPI1;
static cHookJumpT<decltype(&::CreateFileA)> sm_cHookJump_HookAPI2;

struct UNITTEST_N(cHookJump) : public cUnitTest {
    // Make sure this code is not optimized out !
#ifdef _MSC_VER
#pragma optimize("", off)
#endif
    static INT_PTR GRAYCALL HookJump1()  // never inline optimize this
    {
        cUnitTests& uts = cUnitTests::I();
        uts._pLog->addDebugInfoF("cUnitTest_HookJump1");
        return 1;
    }
    static INT_PTR GRAYCALL HookJump2()  // never inline optimize this
    {
        // replace cUnitTest_HookJump1 with this.
        cUnitTests& uts = cUnitTests::I();
        uts._pLog->addDebugInfoF("cUnitTest_HookJump2");
        return 2;
    }
    static void WINAPI Hook_OutputDebugStringA(const char* psz)  // never inline optimize this
    {
        // sm_HookAPI. replace OutputDebugString() with this.

        cHookChain<FARPROC> lock(sm_cHookJump_HookAPI1);
        sm_Hook_Calls++;

        ::OutputDebugStringA("PREFIX WITH CUSTOM:");  // old/raw call.
        ::OutputDebugStringA(psz);                    // old/raw call.

        sm_cHookJump_HookAPI1.GetChainFunc()("Test Chain");  // calls same.
    }
    static HANDLE WINAPI Hook_CreateFileA(_In_ LPCSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ DWORD dwCreationDisposition, _In_ DWORD dwFlagsAndAttributes,
                                          _In_opt_ HANDLE hTemplateFile)  // never inline optimize this
    {
        ::OutputDebugStringA(lpFileName);
        return 0;
    }

#ifdef _MSC_VER
#pragma optimize("", on)  // restore old params.
#endif

    void TestBasic() {
        // Test replacing a internal call.
        INT_PTR iRet = HookJump1();  // test with no hooks.
        UNITTEST_TRUE(iRet == 1);

        cHookJumpT<decltype(&HookJump1)> tester;
        UNITTEST_TRUE(!tester.isHookInstalled());
        HRESULT hRes = tester.InstallHook(HookJump1, HookJump2);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        UNITTEST_TRUE(tester.isHookInstalled());

        iRet = HookJump1();  // really calls cUnitTest_HookJump2
        UNITTEST_TRUE(iRet == 2);

        iRet = HookJump2();
        UNITTEST_TRUE(iRet == 2);

        {
            cHookLock lock(tester);
            iRet = HookJump1();  // cUnitTest_HookJump1 was restored globally.
            UNITTEST_TRUE(iRet == 1);
        }

        iRet = HookJump1();
        UNITTEST_TRUE(iRet == 2);

        {
            cHookChain<FARPROC> lock(tester);
            iRet = HookJump1();  // cUnitTest_HookJump1 still as cUnitTest_HookJump2.
            UNITTEST_TRUE(iRet == 2);
            iRet = lock._pFuncChain();  // cUnitTest_HookJump1 was restored just for Chain.
            UNITTEST_TRUE(iRet == 1);
        }

        iRet = HookJump1();  // cUnitTest_HookJump1 as cUnitTest_HookJump2.
        UNITTEST_TRUE(iRet == 2);

        tester.RemoveHook();
        UNITTEST_TRUE(!tester.isHookInstalled());

        iRet = HookJump1();
        UNITTEST_TRUE(iRet == 1);  // was restored.
        iRet = HookJump2();
        UNITTEST_TRUE(iRet == 2);  // still works as expected.
    }

    void TestAPI1() {
        // Test replacing a call i pull from a system DLL.
        // sm_HookAPI

        auto pFuncOrig = &::OutputDebugStringA;

        UNITTEST_TRUE(sm_Hook_Calls == 0);
        ::OutputDebugStringA("TestAPI() 1 (No prefix)\n");  // raw call.
        UNITTEST_TRUE(sm_Hook_Calls == 0);

        HRESULT hRes = sm_cHookJump_HookAPI1.InstallHook(pFuncOrig, Hook_OutputDebugStringA, true);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        UNITTEST_TRUE(sm_cHookJump_HookAPI1.isHookInstalled());

        ::OutputDebugStringA("TestAPI() call\n");  // raw call. while hooked
        UNITTEST_TRUE(sm_Hook_Calls == 1);

        sm_cHookJump_HookAPI1.RemoveHook();

        ::OutputDebugStringA("TestAPI() 2 (No prefix)\n");  // raw call. while NOT hooked
        UNITTEST_TRUE(sm_Hook_Calls == 1);
    }

    void TestAPI2() {
        auto pFuncOrig = &::CreateFileA;
        // HRESULT hRes = sm_cHookJump_HookAPI2.InstallHook(pFuncOrig, Hook_CreateFileA, true);
        // UNITTEST_TRUE(SUCCEEDED(hRes));
        // UNITTEST_TRUE(sm_cHookJump_HookAPI2.isHookInstalled());

        // watch calls to CreateFileA()
        auto h = ::CreateFileA("c:/tmp/junk.txt", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, cOSHandle::kNULL);

        sm_cHookJump_HookAPI2.RemoveHook();
    }

    UNITTEST_METHOD(cHookJump) {
        //! hook a API function for one call.

        cUnitTests& uts = cUnitTests::I();
        UNITTEST_TRUE(uts._pLog != nullptr);
        // TestAPI2();

        TestBasic();
        // TestAPI1();
    }
};
UNITTEST2_REGISTER(cHookJump, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
