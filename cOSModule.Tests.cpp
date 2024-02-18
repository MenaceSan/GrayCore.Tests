//
//! @file cOSModule.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cOSModule.h>
#include <GrayCore/include/cRandom.h>
#include <GrayCore/include/cSystemHelper.h>

namespace Gray {
struct UNITTEST_N(cOSModule) : public cUnitTest {

    void TestCur() {
#ifndef UNDER_CE
        HMODULE hModCore = cOSModule::GetModuleHandleForAddr(&g_Rand);  // In Core.
        cOSModule modCore(hModCore, cOSModule::k_Load_NoRefCount);
        cStringF sNameCore = modCore.get_Name();  // == "path\\GRAYCORE.DLL"
        UNITTEST_TRUE(modCore.isValidModule());
#endif
    }

    void TestResource() {
        cUnitTests& uts = cUnitTests::I();

        // A system DLL we are sure will NOT be already loaded.
#ifdef _WIN32
        cStringF sDir = cSystemHelper::get_SystemDir();  //  /Windows/System32
        // const FILECHAR_t* k_FileName = _FN("avrt.dll");	// some other ?
        // const char* k_SymName = "AvSetMmThreadPriority";
        const FILECHAR_t* k_FileName = _FN("authz.dll");  // some other ?
        const char* k_SymName = "AuthzFreeHandle";

#elif defined(__linux__)
        cStringF sDir = "/etc/cups";
        cStringF k_FileName = _FN("libcupsmime.so");  //
        const char* k_SymName = "sdfsdfTODO";
#endif

        cOSModule mod2;
        FARPROC pAddr;

        // Not already loaded?
        cOSModule mod;
        bool bRet = mod.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);

        HRESULT hRes = HResult::GetLast();  // ASSUME it fails. 0x8007007e
        UNITTEST_TRUE(!bRet);
        UNITTEST_TRUE(!mod.isValidModule());

#if 1
        hRes = mod.LoadModule(k_FileName, cOSModule::k_Load_Preload);  // does not call DllMain() for process and thread initialization and termination.
        if (FAILED(hRes)) {
            // ERROR_MOD_NOT_FOUND
            uts.m_pLog->addDebugErrorF("Module load 1 ERR='%s'", LOGERR(hRes));
            UNITTEST_TRUE(false);
        }

        UNITTEST_TRUE(mod.isResourceModule());

        bRet = mod2.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);
        UNITTEST_TRUE(bRet);

        pAddr = mod.GetSymbolAddress("qwe");  // known to be bad
        UNITTEST_TRUE(pAddr == nullptr);

        pAddr = mod.GetSymbolAddress(k_SymName);  // known to be in k_FileName. Windows docs say this is not supposed to work ?
        UNITTEST_TRUE(pAddr != nullptr);

        mod.FreeThisModule();  // unload it so we can load it correctly.
        UNITTEST_TRUE(!mod.isValidModule());

        // Unloaded.
        bRet = mod2.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);
        UNITTEST_TRUE(!bRet);
#endif

        // Now load as a normal module.
        hRes = mod.LoadModule(k_FileName);
        if (FAILED(hRes)) {
            // ERROR_MOD_NOT_FOUND
            uts.m_pLog->addDebugErrorF("Module load 2 ERR='%s'", LOGERR(hRes));
            UNITTEST_TRUE(false);
        }
        UNITTEST_TRUE(mod.isValidModule());
        UNITTEST_TRUE(!mod.isResourceModule());

        bRet = mod2.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);
        UNITTEST_TRUE(bRet);

        pAddr = mod.GetSymbolAddress(k_SymName);
        UNITTEST_TRUE(pAddr != nullptr);

        mod.FreeThisModule();
        UNITTEST_TRUE(!mod.isValidModule());

        // Unloaded?
        bRet = mod2.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);
        UNITTEST_TRUE(!bRet);  // no longer present.
    }

    UNITTEST_METHOD(cOSModule) {
        //! Load some system DLL/SO?
        //! get_TestInpDir()

        cUnitTests& uts = cUnitTests::I();

        TestCur();

        cOSModule modCur(cAppState::get_HModule(), cOSModule::k_Load_NoRefCount);
        cStringF sNameCur = modCur.get_Name();  // .EXE name
        if (sNameCur.IsEmpty()) {          // App EXE name is empty for some reason?
            uts.m_pLog->addDebugErrorF("Module name ERR='%s'", LOGERR(HResult::GetLast()));
            UNITTEST_TRUE(false);
            return;
        }

        TestResource();
    }
};
UNITTEST2_REGISTER(cOSModule, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
