//
//! @file cAppState.Tests.cpp
//
#include "pch.h"
#include "cAppState.h"
#include "cString.h"

namespace Gray
{
	UNITTEST2_CLASS(cAppState)
	{ 
		UNITTEST2_METHOD(cAppState)
		{
			cUnitTestAppState inmain;
			cAppState& app = cAppState::I();

			cAppArgs args2;
			args2.InitArgsF(_FN(""));

			args2.InitArgsF(_FN("a b c"));

			args2.InitArgsF(_FN("a=1 b=2 c=3"));

			args2.InitArgsF(_FN("a='sdf sdf' b='123123' c='sdf sdf sdf sdf '"));

#ifdef _WIN32
			_IMAGE_DOS_HEADER* pHeader = (_IMAGE_DOS_HEADER*)app.get_HModule();			//!< the current applications instance handle/base address. _IMAGE_DOS_HEADER
			UNITTEST2_TRUE(pHeader != nullptr);
#endif

			auto sUserName = app.GetCurrentUserName();
			UNITTEST2_TRUE(!sUserName.IsEmpty());

			UNITTEST2_TRUE(app.isAppRunning());	// cAppStateMain was called!
			DEBUG_MSG(("Arg Qty = %d", app.m_Args.get_ArgsQty()));

			for (int i = 0; i < app.m_Args.get_ArgsQty(); i++)
			{
				cStringF sArg = app.m_Args.GetArgsEnum(i);
			}

			cArrayString<FILECHAR_t> aEnv;
			app.GetEnvironArray(aEnv);
			UNITTEST2_TRUE(aEnv.GetSize());
			DEBUG_MSG(("Env Qty = %d", aEnv.GetSize()));

			cStringF sCurrentDir = app.get_CurrentDir();	// just testing.
			DEBUG_MSG(("Current Dir = '%s'", LOGSTR(sCurrentDir)));

			cStringF sDirTmp = app.get_TempDir();
			UNITTEST2_TRUE(sDirTmp.GetLength() > 0);
			DEBUG_MSG(("Temp Dir = '%s'", LOGSTR(sDirTmp)));

		}
	};

	UNITTEST2_REGISTER(cAppState, UNITTEST_LEVEL_Core);
}
