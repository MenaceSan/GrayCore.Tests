//! @file cAppConsole.Tests.cpp
// clang-format off
#include "pch.h"
// clang-format on
#include <GrayCore/include/StrCharAscii.h>
#include <GrayCore/include/cAppConsole.h>

namespace Gray {
struct UNITTEST_N(cAppConsole) : public cUnitTest {
    UNITTEST_METHOD(cAppConsole) {
        cUnitTests& uts = cUnitTests::I();
        cAppConsole& console = cAppConsole::I();
        if (!console.isConsoleMode()) {
            if (uts._pLog != nullptr) {
                uts._pLog->addDebugInfoF("cAppConsole is NOT in CONSOLE MODE");
            }
            // try to create or attach a console using AllocConsole() ??
            console.AttachOrAllocConsole();
        }

        console.WriteString(_GT("cAppConsole in CONSOLE MODE" STR_NL));

        if (uts.IsTestInteractive()) {
            console.WriteString(_GT("Press ESC to continue." STR_NL));

            console.SetKeyModes(false, false);
            for (int i = 0; i < 20; i++) {
                const int iKey = console.ReadKeyWait();
                if (iKey == static_cast<int>(ASCII_t::_ESC))  // ESC = 27
                    break;
                console.Printf(_GT("Got Key %d='%c'." STR_NL), iKey, iKey);
                cThreadId::SleepCurrent(1);
            }
            console.SetKeyModes();  // restore modes to default.
        }
        console.ReleaseConsole();
    }
};
UNITTEST2_REGISTER(cAppConsole, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
