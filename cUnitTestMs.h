//
// cUnitTestMs.h
// ONLY included from a test DLL.

#pragma once
#include "../GrayCore/include/cUnitTest.h"
#include "../GrayCore/include/cLogMgr.h"
#include "../GrayCore/include/cUniquePtr.h"

	// use _LIB && _WINDLL && _MFC_VER to identify the type of LIB build. or it may just be who is including us.
#ifndef GRAYCORE_TEST_LINK
#if defined(_MFC_VER) || defined(GRAY_STATICLIB)	// GRAY_STATICLIB or _MFC_VER can be defined to make Gray* all static lib
#define GRAYCORE_TEST_LINK
#else
#define GRAYCORE_TEST_LINK __DECL_IMPORT	// default is to include from a DLL/SO  
#endif
#endif

#ifdef USE_UNITTESTS_MS		// register with M$ unit test framework as well. define in global namespace.
#include <CppUnitTest.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

namespace Gray
{

#ifdef USE_UNITTESTS_MS		// _WIN32 should register with M$ unit test framework as well. define in global namespace.

// #define UNITTEST_TRUE(x) 		::Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(x) 
// #define UNITTEST_TRUE2(x, d)	::Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(x)

#define UNITTEST_MSN(n)	n##TestsMS // UNITTEST_N(n) ## MS
#define UNITTEST2_REGISTER(n,lvl)	UNITTEST_REGISTER(n,lvl); UNITTEST_EXT_EXP(n); TEST_CLASS(UNITTEST_MSN(n)) { public: cUniquePtr<UNITTEST_N(n)> m_pTest; UNITTEST_MSN(n)() : m_pTest(new UNITTEST_N(n)) {} TEST_METHOD(RunTest) { m_pTest->RunUnitTest(); } }

#else

#define UNITTEST2_REGISTER(n,lvl) 		UNITTEST_REGISTER(n,lvl); UNITTEST_EXT_EXP(n);
#endif

}
