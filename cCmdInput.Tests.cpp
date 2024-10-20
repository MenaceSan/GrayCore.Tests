//! @file cCmdInput.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cCmdInput.h>
#include <GrayCore/include/StrCharAscii.h>

namespace Gray {
struct UNITTEST_N(cCmdInput) : public cUnitTest {
    UNITTEST_METHOD(cCmdInput) {
        cCmdInput cmdlog;
        for (int i = 0; i < _countof(k_asTextLines); i++) {
            cmdlog.AddCmd(k_asTextLines[i]);
        }

        HRESULT hRes;
        UNITTEST_TRUE(cmdlog.get_CmdQty() == _countof(cUnitTests::k_asTextLines));
        // cmdlog

        hRes = cmdlog.AddInputKey('a', nullptr, false);
        UNITTEST_TRUE(hRes == 1);
        hRes = cmdlog.AddInputKey('b', nullptr, false);
        UNITTEST_TRUE(hRes == 1);
        hRes = cmdlog.AddInputKey('\n', nullptr, false);
        UNITTEST_TRUE(hRes == 2);
        UNITTEST_TRUE(cmdlog._sCmd == "ab");

        hRes = cmdlog.AddInputKey('c', nullptr, false);
        UNITTEST_TRUE(hRes == 1);
        hRes = cmdlog.AddInputKey('d', nullptr, false);
        UNITTEST_TRUE(hRes == 1);
        hRes = cmdlog.AddInputKey('\n', nullptr, false);
        UNITTEST_TRUE(hRes == 2);
        UNITTEST_TRUE(cmdlog._sCmd == "cd");

        hRes = cmdlog.AddInputKey(cCmdInput::kKeyUp);
        UNITTEST_TRUE(hRes == 0);
        UNITTEST_TRUE(cmdlog._sCmd == "ab");
        hRes = cmdlog.AddInputKey(cCmdInput::kKeyUp);
        UNITTEST_TRUE(hRes == 0);
        UNITTEST_TRUE(cmdlog._sCmd == "my");  // k_asTextLines
        hRes = cmdlog.AddInputKey((char)ASCII_t::_ESC);
        UNITTEST_TRUE(hRes == 0);
        UNITTEST_TRUE(cmdlog._sCmd == "");
        hRes = cmdlog.AddInputKey(cCmdInput::kKeyDown);
        UNITTEST_TRUE(hRes == 0);
        UNITTEST_TRUE(cmdlog._sCmd == "cd");

        hRes = cmdlog.AddInputKey('\n', nullptr, false);
        UNITTEST_TRUE(hRes == 2);

        UNITTEST_TRUE(cmdlog._sCmd == "cd");
    }
};
UNITTEST2_REGISTER(cCmdInput, UNITTEST_LEVEL_t::_Lib);
}  // namespace GrayLib
