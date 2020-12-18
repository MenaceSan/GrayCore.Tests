//
// cUnitTestMs.h
//

#pragma once
#include "../GrayCore/include/cUnitTest.h"
#include "../GrayCore/include/cLogMgr.h"

	// use _LIB && _WINDLL && _MFC_VER to identify the type of LIB build. or it may just be who is including us.
#ifndef GRAYCORE_TEST_LINK
#if defined(_MFC_VER) || defined(GRAY_STATICLIB)	// GRAY_STATICLIB or _MFC_VER can be defined to make Gray* all static lib
#define GRAYCORE_TEST_LINK
#else
#define GRAYCORE_TEST_LINK __DECL_IMPORT	// default is to include from a DLL/SO (GRAY_DLL)
#endif
#endif

#ifdef USE_UNITTESTS_MS		// register with M$ unit test framework as well. define in global namespace.
#include <CppUnitTest.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

namespace Gray
{

#ifdef USE_UNITTESTS_MS		// _WIN32 should register with M$ unit test framework as well. define in global namespace.

#define UNITTEST2_CLASS(n)			TEST_CLASS( UNITTEST_N(n)) , public cUnitTest
#define UNITTEST2_METHOD(x)			public: virtual ~UNITTEST_N(x)() noexcept(false) {} TEST_METHOD(RunTest) { this->RunUnitTest(); } virtual void RunUnitTest() override
#define UNITTEST2_REGISTER(n,x)		// make a static instance of the class to register itself. for non-M$ test
#define UNITTEST2_TRUE(x)			Assert::IsTrue(x)

#else

#define UNITTEST2_CLASS(n)		class UNITTEST_N(n) : public cUnitTest //!< define and implement class. TEST_CLASS(n)
#define UNITTEST2_METHOD(x)		public: virtual void UnitTest() override				// call the public virtual as a test. TEST_METHOD(x)
#define UNITTEST2_REGISTER(n,x)		// make a static instance of the class to register itself. for non-M$ test
#define UNITTEST2_TRUE(x)			// (x)

#endif

}
