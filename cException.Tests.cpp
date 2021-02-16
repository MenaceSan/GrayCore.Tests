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

	UNITTEST2_CLASS(cException)
	{
	public:

		void TestThrow1()
		{
			static const char* k_pszMsg = "Test Exception";
			GRAY_TRY
			{
				GRAY_THROW cUnitTestException(k_pszMsg, LOGLEV_TRACE);	// ignore this throw.
			}
				GRAY_TRY_CATCH(cExceptionBase, ex)
			{
				// We should get here.
				cExceptionHolder exh(ex);	// kill it.
				UNITTEST2_TRUE(exh.get_Severity() == LOGLEV_TRACE);
				cStringL sMsg = exh.get_ErrorStr();	// may have prefix.
				UNITTEST2_TRUE(sMsg.Contains(k_pszMsg));
				return;	// done.
			}
#ifndef _MFC_VER
			GRAY_TRY_CATCHALL
			{
				UNITTEST2_TRUE(false);	// should not get here !
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

		UNITTEST2_METHOD(cException)
		{
			TestThrow1();
			TestSystem();
		}
	};

	UNITTEST2_REGISTER(cException, UNITTEST_LEVEL_Core);
}

#else
			// No exceptions allowed
#endif
