//
// cUnitTestMs.h
//

#pragma once
#include "../GrayCore/include/cUnitTest.h"
#include "../GrayCore/include/cLogMgr.h"
#include "../GrayCore/include/cNewPtr.h"

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

#define UNITTEST2_CLASS(n)		class UNITTEST_N(n) : public cUnitTest //!< define and implement class.
#define UNITTEST2_METHOD(x)		public: void RunUnitTest() override				// call the public virtual as a test. 

#ifdef USE_UNITTESTS_MS		// _WIN32 should register with M$ unit test framework as well. define in global namespace.

// #define UNITTEST2_TRUE 			::Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue 
// #define UNITTEST2_TRUE2(x, d)	::Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(x)
#define UNITTEST2_REGISTER(n,l)	UNITTEST_REGISTER(n,l); TEST_CLASS(n##TestsMS) { public: cNewPtr<cUnitTest> m_pTest; n##TestsMS() : m_pTest(new UNITTEST_N(n)) {} TEST_METHOD(RunTest) { m_pTest->RunUnitTest(); } }

#else

#define UNITTEST2_REGISTER 		UNITTEST_REGISTER  
#endif

#define UNITTEST2_TRUE 			UNITTEST_TRUE 
#define UNITTEST2_TRUE2(x, d)	UNITTEST_TRUE2

}
