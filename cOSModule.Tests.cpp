//
//! @file cOSModule.Tests.cpp
//
#include "pch.h"
#include "cOSModule.h"
#include "cRandom.h"
#include "cSystemHelper.h"

namespace Gray
{
	UNITTEST2_CLASS(cOSModule)
	{
		UNITTEST2_METHOD(cOSModule)
		{
 			//! Load some system DLL/SO?
			//! get_TestInpDir()

			cUnitTests& uts = cUnitTests::I();

#ifndef UNDER_CE
			HMODULE hModCore = cOSModule::GetModuleHandleForAddr(&g_Rand);	// In Core.
			cOSModule modCore(hModCore, cOSModule::k_Load_NoRefCount);
			cStringF sNameCore = modCore.get_Name(); // == "path\\GRAYCORE.DLL"
			UNITTEST2_TRUE(modCore.isValidModule());
#endif

			cOSModule modCur(cAppState::get_HModule(), cOSModule::k_Load_NoRefCount);
			cStringF sNameCur = modCur.get_Name();		// .EXE name
			if (sNameCur.GetLength() == 0)	// App EXE name is empty for some reason?
			{
				uts.m_pLog->addDebugErrorF("Module name ERR='%s'", LOGERR(HResult::GetLast()));
				UNITTEST2_TRUE(false);
				return;
			}

			// A system DLL we are sure will NOT be already loaded.
#ifdef _WIN32
			const FILECHAR_t* k_FileName = _FN("avrt.dll");	// some other ? 
			cStringF sDir = cSystemHelper::get_SystemDir();	//  /Windows/System32
			const char* k_SymName = "AvSetMmThreadPriority";
#elif defined(__linux__)
			cStringF k_FileName = _FN("libcupsmime.so");		// 
			cStringF sDir = "/etc/cups";
			const char* k_SymName = "sdfsdf";
#endif

			cOSModule mod2;
			FARPROC pAddr;

			// Not already loaded?
			cOSModule mod;
			bool bRet = mod.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);
			UNITTEST2_TRUE(!bRet);
			UNITTEST2_TRUE(!mod.isValidModule());

			HRESULT hRes;
#if 1
			hRes = mod.LoadModule(k_FileName, cOSModule::k_Load_Preload);	// does not call DllMain() for process and thread initialization and termination.
			if (FAILED(hRes))
			{
				// ERROR_MOD_NOT_FOUND
				uts.m_pLog->addDebugErrorF("Module load 1 ERR='%s'", LOGERR(hRes));
				UNITTEST2_TRUE(false);
			}

			UNITTEST2_TRUE(mod.isResourceModule());

			bRet = mod2.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);
			UNITTEST2_TRUE(bRet);

			pAddr = mod.GetSymbolAddress("qwe");		// known to be bad
			UNITTEST2_TRUE(pAddr == nullptr);

			pAddr = mod.GetSymbolAddress(k_SymName);		// known to be good.
			UNITTEST2_TRUE(pAddr != nullptr);

			mod.FreeThisModule();	// unload it so we can load it correctly.
			UNITTEST2_TRUE(!mod.isValidModule());

			// Unloaded.
			bRet = mod2.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);
			UNITTEST2_TRUE(!bRet);
#endif

			// Now load as a normal module.
			hRes = mod.LoadModule(k_FileName);
			if (FAILED(hRes))
			{
				// ERROR_MOD_NOT_FOUND
				uts.m_pLog->addDebugErrorF("Module load 2 ERR='%s'", LOGERR(hRes));
				UNITTEST2_TRUE(false);
			}
			UNITTEST2_TRUE(mod.isValidModule());
			UNITTEST2_TRUE(!mod.isResourceModule());

			bRet = mod2.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);
			UNITTEST2_TRUE(bRet);

			pAddr = mod.GetSymbolAddress(k_SymName);
			UNITTEST2_TRUE(pAddr != nullptr);

			mod.FreeThisModule();
			UNITTEST2_TRUE(!mod.isValidModule());

			// Unloaded?
			bRet = mod2.AttachModuleName(k_FileName, cOSModule::k_Load_NoRefCount);
			UNITTEST2_TRUE(!bRet);

		}
	};
	UNITTEST2_REGISTER(cOSModule, UNITTEST_LEVEL_Core);
}
