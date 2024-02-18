//! @file cAppImpl.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cAppImpl.h>

namespace Gray {
/// <summary>
/// Create a test instance of cAppCommand
/// </summary>
struct cAppImplTestCmd : public cAppCommand {
    const CommandId_t m_nTestNum;
    cStringA m_sName;    /// symbolic name for -switch or /switch (case inSensitive). MUST be unique.
    cStringF m_sSwitch;  /// abbreviated -switch or /switch (case Sensitive) optional, nullptr allowed
    int m_nCalled = 0;

    cAppImplTestCmd(CommandId_t nTestNum) : m_nTestNum(nTestNum), cAppCommand(m_sSwitch = cStringA::GetFormatf("t%d", nTestNum), m_sName = cStringA::GetFormatf("Test%d", nTestNum), nullptr, "Test command") {}

    HRESULT DoCommand(const cAppArgs& args, int iArgN = 0) override {  /// call this if we see the m_pszCmd switch. can consume more arguments (or not).
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
        bool created = false;
        cAppImpl* pInst = cAppImpl::get_SingleU();
        ITERATE_t argStart = 0;
        if (pInst == nullptr) {
            // We are running as a test DLL with no cAppImpl.
            pInst = new cAppImpl(_FN("TestApp"));
            created = true;
            pInst->m_State.InitArgsWin(_FN("-t1=234234 -t2 123 -t2=234 -t1"));
        } else {
            // Add command calls to the end of the existing command line.
            cAppArgs& argSet = pInst->m_State.m_Args;
            argStart = argSet.AppendArg(_FN("-t1=234234"), true);  // s_t1
            argSet.AppendArg(_FN("-t2"), true);         // s_t2
            argSet.AppendArg(_FN("123"), false);
            argSet.AppendArg(_FN("-t2=234"), true);
            argSet.AppendArg(_FN("-t1"), false);
        }

        static cAppImplTestCmd s_t1(1);
        pInst->_Commands.RegisterCommand(s_t1);
        static cAppImplTestCmd s_t2(2);
        pInst->_Commands.RegisterCommand(s_t2);
        static cAppImplTestCmd s_t3(3);  // never called.
        pInst->_Commands.RegisterCommand(s_t3);

        HRESULT hRes = pInst->RunCommands();    // This might be re-entrant !?
        UNITTEST_TRUE(SUCCEEDED(hRes));

        // Check m_ArgsValid
        const UINT validBits = pInst->m_State.m_ArgsValid;
        // UNITTEST_TRUE(validBits == 0xFE);

        UNITTEST_TRUE(s_t1.m_nCalled == 2);
        UNITTEST_TRUE(s_t2.m_nCalled == 2);
        UNITTEST_TRUE(s_t3.m_nCalled == 0);
    }
};

UNITTEST2_REGISTER(cAppImpl, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
