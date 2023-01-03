//
//! @file cException.Tests.cpp
//
#include "pch.h"
#include "cException.h"
#include "cString.h"
#include "cExceptionSystem.h"

#ifdef _CPPUNWIND

namespace Gray
{
	class cUnitTestException : public cException
	{
	public:
		cUnitTestException(const char* pszMsg, LOGLEV_TYPE eLogLevel)
			: cException(pszMsg, eLogLevel)
		{
		}
	};

	UNITTEST_CLASS(cException)
	{
	public:
		void TestSomeJumper(cExceptionJmp& jmp) noexcept
		{
			char szStackTest2[123];
			StrT::CopyLen(szStackTest2, "JUNK", _countof(szStackTest2));
			// Do the longjmp()
			jmp.Jump(1);
		}

		void TestJmp()
		{
			// cExceptionJmp

			static const char k_TestVal[4] = "123";

			char szStackTest[123];
			const char* pszStackTest = szStackTest;
			StrT::CopyLen(szStackTest, k_TestVal, _countof(szStackTest));

			cExceptionJmp jmp1;

			int ret = jmp1.Init();
			switch (ret)
			{
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

			UNITTEST_TRUE(ret==1);

			// Is the stack ok ?
			UNITTEST_TRUE(pszStackTest == szStackTest);
			UNITTEST_TRUE(StrT::Cmp(pszStackTest, k_TestVal) == 0);
		}

		void TestThrow1()
		{
			static const char* k_pszMsg = "Test Exception";
			GRAY_TRY
			{
				GRAY_THROW cUnitTestException(k_pszMsg, LOGLEV_TRACE);	// ignore this throw in unit test.
			}
				GRAY_TRY_CATCH(cExceptionBase, ex)
			{
				// We should get here.
				cExceptionHolder exh(ex);	// kill it.
				UNITTEST_TRUE(exh.get_Severity() == LOGLEV_TRACE);
				cStringL sMsg = exh.get_ErrorStr();	// may have prefix.
				UNITTEST_TRUE(sMsg.Contains(k_pszMsg));
				return;	// done.
			}
#ifndef _MFC_VER
			GRAY_TRY_CATCHALL
			{
				UNITTEST_TRUE(false);	// should not get here !
			}
#endif
				GRAY_TRY_END
				UNREACHABLE_CODE(__noop);	// should never get here.
		}

		void TestSystem()
		{
			// Test cExceptionSystem
#ifdef _MSC_VER
			__try
			{
				// do bad stuff.

				// nullptr reference.

				// divide by zero. RPC_S_ZERO_DIVIDE
			}
			__except (cExceptionSystem::FilterException(GetExceptionCode(), GetExceptionInformation()))
			{
				// like catch
				//-- display the fatal error message
#if 0
				MessageBox(0, "Exception was caught here!",
					"Unexpected Error", MB_OK);
#endif

				DEBUG_MSG(("Exception Test"));
			}
#endif
			// TODO a try/catch block as well.

			// cExceptionSystem::InitForCurrentThread()
		}

		UNITTEST_METHOD(cException)
		{
			// TestJmp(); // M$ VS has a bug that prevents this from working in some cases.
			TestThrow1();
			TestSystem();
		}
	};

	UNITTEST2_REGISTER(cException, UNITTEST_LEVEL_Core);
}

#else
			// No exceptions allowed
#endif
