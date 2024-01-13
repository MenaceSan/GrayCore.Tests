//
//! @file cAppState.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cAppState.h>
#include <GrayCore/include/cString.h>

namespace Gray {
struct UNITTEST_N(cAppState) : public cUnitTest {
    UNITTEST_METHOD(cAppState) {
        cUnitTestAppState inmain;
        cAppState& app = cAppState::I();

        cAppArgs args2;
        args2.InitArgsWin(_FN(""));

        args2.InitArgsWin(_FN("a b c"));

        args2.InitArgsWin(_FN("a=1 b=2 c=3"));

        args2.InitArgsWin(_FN("a='sdf sdf' b='123123' c='sdf sdf sdf sdf '"));

#ifdef _WIN32
        _IMAGE_DOS_HEADER* pHeader = (_IMAGE_DOS_HEADER*)app.get_HModule();  //!< the current applications instance handle/base address. _IMAGE_DOS_HEADER
        UNITTEST_TRUE(pHeader != nullptr);
#endif

        auto sUserName = app.GetCurrentUserName();
        UNITTEST_TRUE(!sUserName.IsEmpty());

        UNITTEST_TRUE(app.isAppRunning());  // cAppStateMain was called!
        DEBUG_MSG(("Arg Qty = %d", app.m_Args.get_ArgsQty()));

        for (int i = 0; i < app.m_Args.get_ArgsQty(); i++) {
            cStringF sArg = app.GetArgEnum(i);
        }

        cArrayString<FILECHAR_t> aEnv;
        app.GetEnvironArray(aEnv);
        UNITTEST_TRUE(aEnv.GetSize());
        DEBUG_MSG(("Env Qty = %d", aEnv.GetSize()));

        cStringF sCurrentDir = app.get_CurrentDir();  // just testing.
        DEBUG_MSG(("Current Dir = '%s'", LOGSTR(sCurrentDir)));

        cStringF sDirTmp = app.get_TempDir();
        UNITTEST_TRUE(sDirTmp.GetLength() > 0);
        DEBUG_MSG(("Temp Dir = '%s'", LOGSTR(sDirTmp)));
    }
};
UNITTEST2_REGISTER(cAppState, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
