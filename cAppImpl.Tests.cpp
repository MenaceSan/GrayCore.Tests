//! @file cAppImpl.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cAppImpl.h>

namespace Gray {
/// <summary>
/// Create a test instance of cAppCommand
/// </summary>
struct cAppImplTestCmd final : public cAppCommand {
    const CommandId_t m_nTestNum;
    cStringA _sName;    /// symbolic name for -switch or /switch (case inSensitive). MUST be unique.
    cStringF m_sSwitch;  /// abbreviated -switch or /switch (case Sensitive) optional, nullptr allowed
    mutable int m_nCalled = 0;

    cAppImplTestCmd(CommandId_t nTestNum) : m_nTestNum(nTestNum), cAppCommand(m_sSwitch = cStringA::GetFormatf("t%d", nTestNum), _sName = cStringA::GetFormatf("Test%d", nTestNum), nullptr, "Test command") {}

    HRESULT DoCommand(const cAppArgs& args, int iArgN = 0)   override {  /// call this if we see the m_pszCmd switch. can consume more arguments (or not).
        const auto sArgMod = args.GetArgEnum(iArgN);
        const bool hasArgMod = cAppArgs::IsArgMod(sArgMod);
        cLogMgr::I().addInfoF("Test command handler %d", m_nTestNum);
        m_nCalled++;
        return hasArgMod ? 1 : 0;
    }
};

struct UNITTEST_N(cAppImpl) : public cUnitTest {
    // add some task command handlers.

    UNITTEST_METHOD(cAppImpl) {
        static bool sm_created = false;
        cAppImpl* pApp = cAppImpl::get_SingleU();
        ITERATE_t argStart = 0;
        if (pApp == nullptr) {
            // We are running as a test DLL with no cAppImpl.
            pApp = new cAppImpl(_FN("TestApp"));    // will leak.
            UNITTEST_TRUE(cAppImpl::isSingleCreated());
            sm_created = true;
            auto& rState = cAppState::I();
            rState.InitArgsWin(_FN("-t1=234234 -t2 123 -t2=234 -t1"));
        } else if (!sm_created) {
            // Add command calls to the end of the existing command line.
            auto& rState = cAppState::I();
            argStart = rState._Args.AppendArg(_FN("-t1=234234"), true);  // s_t1
            rState._Args.AppendArg(_FN("-t2"), true);         // s_t2
            rState._Args.AppendArg(_FN("123"), false);
            rState._Args.AppendArg(_FN("-t2=234"), true);
            rState._Args.AppendArg(_FN("-t1"), false);
            sm_created = true;
        }

        cBitmask<> validPrev = pApp->_ArgsValid;

        static cAppImplTestCmd s_t1(1);
        pApp->_Commands.RegisterCommand(s_t1);

        static cAppImplTestCmd s_t2(2);
        pApp->_Commands.RegisterCommand(s_t2);

        static cAppImplTestCmd s_t3(3);  // never called.
        pApp->_Commands.RegisterCommand(s_t3);

        HRESULT hRes = pApp->RunCommands();    // This might be re-entrant!
        UNITTEST_TRUE(SUCCEEDED(hRes));

        // Check _ArgsValid
        const UINT validBits = pApp->_ArgsValid;
        UNITTEST_TRUE(validPrev != validBits);
        // UNITTEST_TRUE(validBits == 0xFE);

        UNITTEST_TRUE(s_t1.m_nCalled == 2);
        UNITTEST_TRUE(s_t2.m_nCalled == 2);
        UNITTEST_TRUE(s_t3.m_nCalled == 0);

        // restore state so this may be called again.
        s_t1.m_nCalled = 0;
        s_t2.m_nCalled = 0;
        s_t3.m_nCalled = 0;
        pApp->_ArgsValid = validPrev;
    }
};

UNITTEST2_REGISTER(cAppImpl, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
