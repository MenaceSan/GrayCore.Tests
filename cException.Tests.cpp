//
//! @file cException.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cException.h>
#include <GrayCore/include/cExceptionSystem.h>
#include <GrayCore/include/cString.h>
#include <GrayCore/include/StrBuilder.h>

#ifdef _CPPUNWIND

namespace Gray {
struct cUnitTestException : public cException {
    cUnitTestException(const char* pszMsg, LOGLVL_t eLogLevel) : cException(pszMsg, eLogLevel) {}
};

struct UNITTEST_N(cException) : public cUnitTest {
    void TestSomeJumper(cExceptionJmp& jmp) noexcept {
        char szStackTest2[123];
        StrT::CopyLen(szStackTest2, "JUNK", _countof(szStackTest2));
        // Do the longjmp()
        jmp.Jump(1);
    }

    void TestJmp() {
        // cExceptionJmp

        static const char k_TestVal[4] = "123";

        char szStackTest[123];
        const char* pszStackTest = szStackTest;
        StrT::CopyLen(szStackTest, k_TestVal, _countof(szStackTest));

        cExceptionJmp jmp1;

        int ret = jmp1.Init();
        switch (ret) {
            case 0:
                // this is dangerous in C++. Unwin
                TestSomeJumper(jmp1);
                // Will NEVER get here.
                UNITTEST_TRUE(false);
                break;
            case 1:
                // This is the longjump() return case.
                break;
            default:
                UNITTEST_TRUE(false);
                break;
        }

        UNITTEST_TRUE(ret == 1);

        // Is the stack ok ?
        UNITTEST_TRUE(pszStackTest == szStackTest);
        UNITTEST_TRUE(StrT::Cmp(pszStackTest, k_TestVal) == 0);
    }

    void TestThrow1() {
        static const char* k_pszMsg = "Test Exception";
        GRAY_TRY {
            GRAY_THROW cUnitTestException(k_pszMsg, LOGLVL_t::_TRACE);  // ignore this throw in unit test.
        }
        GRAY_TRY_CATCH(cExceptionBase, ex) {
            // We should get here.
            cExceptionHolder exh(ex);  // kill it.
            UNITTEST_TRUE(exh.get_Severity() == LOGLVL_t::_TRACE);
            cStringL sMsg = exh.get_ErrorStr();  // may have prefix.
            UNITTEST_TRUE(sMsg.Contains(k_pszMsg));
            return;  // done.
        }
#ifndef _MFC_VER
        GRAY_TRY_CATCHALL {
            UNITTEST_TRUE(false);  // should not get here !
        }
#endif
        GRAY_TRY_END;
        UNREACHABLE_CODE(__noop);  // should never get here.
    }

    void TestSystem() {
        // Test cExceptionSystem
#ifdef _MSC_VER
        __try {
            // do bad stuff.

            // nullptr reference.

            // divide by zero. RPC_S_ZERO_DIVIDE
        } __except (cExceptionSystem::FilterException(GetExceptionCode(), GetExceptionInformation())) {
            // like catch
            //-- display the fatal error message
#if 0
			ShowMessageBox(0, "Exception was caught here!",
					"Unexpected Error", MB_OK);
#endif

            DEBUG_MSG(("Exception Test"));
        }
#endif
        // TODO a try/catch block as well.

        // cExceptionSystem::InitForCurrentThread()
    }

    void TestHResult(){
        cExceptionHResult ehr;
        ehr.m_hResultCode = E_FAIL;
        StrBuilder<GChar_t> sb;
        ehr.GetErrorMessage(sb, nullptr);
        UNITTEST_TRUE(sb.get_Length() > 2);
    }

    UNITTEST_METHOD(cException) {
        // TestJmp(); // M$ VS has a bug that prevents this from working in some cases.
        TestThrow1();
        TestSystem();
        TestHResult();
    }
};

UNITTEST2_REGISTER(cException, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray

#else
// No exceptions allowed
#endif
