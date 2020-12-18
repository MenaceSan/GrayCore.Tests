// 
//! @file cTypeInfo.Tests.cpp
//
#include "pch.h"
#include "cTypeInfo.h"
#include "cTypes.h"

namespace Gray
{
	DECLARE_INTERFACE(ITestClass0)
	{
		IGNORE_WARN_INTERFACE(ITestClass0);
		virtual void f1() = 0;
	};

	UNITTEST2_CLASS(cTypeInfo)
	{
		class cTestClass1 : public ITestClass0
		{
			virtual void f1() = 0;
		};
		class cTestClass2 : public ITestClass0
		{
			virtual void f1() = 0;
			virtual void f2() = 0;
		};
		class cTestClass3 : public cTestClass2, public cTestClass1
		{
			virtual void f1()
			{
			}
			virtual void f2()
			{
			}
			virtual void f3()
			{
			}
		};

		template< typename TYPE >
		void TestFloatTypes()
		{
			const TYPE kNaN = NAN;
			const TYPE kPInf = INFINITY;
			const TYPE kNInf = -INFINITY;

			UNITTEST2_TRUE(cTypeFloat::IsFinite<TYPE>(0));
			UNITTEST2_TRUE(cTypeFloat::IsFinite<TYPE>(123));
			UNITTEST2_TRUE(cTypeFloat::IsFinite<TYPE>((TYPE)123.456));
			UNITTEST2_TRUE(!cTypeFloat::IsFinite<TYPE>(kNaN));
			UNITTEST2_TRUE(!cTypeFloat::IsFinite<TYPE>(kPInf));
			UNITTEST2_TRUE(!cTypeFloat::IsFinite<TYPE>(kNInf));

			UNITTEST2_TRUE(!cTypeFloat::IsNaN<TYPE>(0));
			UNITTEST2_TRUE(!cTypeFloat::IsNaN<TYPE>(123));
			UNITTEST2_TRUE(!cTypeFloat::IsNaN<TYPE>((TYPE)123.456));
			UNITTEST2_TRUE(cTypeFloat::IsNaN<TYPE>(kNaN));
			UNITTEST2_TRUE(!cTypeFloat::IsNaN<TYPE>(kPInf));
			UNITTEST2_TRUE(!cTypeFloat::IsNaN<TYPE>(kNInf));

			UNITTEST2_TRUE(!cTypeFloat::IsInfinite<TYPE>(0));
			UNITTEST2_TRUE(!cTypeFloat::IsInfinite<TYPE>(123));
			UNITTEST2_TRUE(!cTypeFloat::IsInfinite<TYPE>((TYPE)123.456));
			UNITTEST2_TRUE(!cTypeFloat::IsInfinite<TYPE>(kNaN));
			UNITTEST2_TRUE(cTypeFloat::IsInfinite<TYPE>(kPInf));
			UNITTEST2_TRUE(cTypeFloat::IsInfinite<TYPE>(kNInf));
		}

		void TestInt()
		{
			int nInt = 0;
			const TYPEINFO_t& tInt0 = typeid(int);
			const TYPEINFO_t& tInt1 = typeid(nInt);
			// const TYPEINFO_t& tInt2 = typeid( decltype(nInt));		// Test use of C++11 feature "-Wc++0x-compat"
			UNITTEST2_TRUE(tInt0 == tInt1);
			// UNITTEST2_TRUE(tInt0 == tInt2);
			UNITTEST2_TRUE(nInt == 0);	// just for a ref.
		}

		void TestRef()
		{
			const cTypeInfo& tRef0 = (const cTypeInfo&) typeid(cRefBase);
			size_t h0 = tRef0.get_HashCode();
			UNITTEST2_TRUE(h0 != 0);

			cRefBase oSmart;
			const cTypeInfo& tRef1 = (const cTypeInfo&) typeid(oSmart);
			size_t h1 = tRef1.get_HashCode();
			UNITTEST2_TRUE(h0 == h1);	// same type

#ifdef _MSC_VER
			const char* pszRawName1 = tRef1.raw_name();	// Get M$ Mangled/Raw name ".?AVcRefBase@Gray@@"
			UNITTEST2_TRUE(pszRawName1 != nullptr);
#endif

			const char* pszName1 = tRef1.get_Name();		// "class Gray::cRefBase"
			UNITTEST2_TRUE(pszName1 != nullptr);
		}

		void TestClass1()
		{
			cTestClass3 oTC3;
			const cTypeInfo& tTC3 = (const cTypeInfo&) typeid(oTC3);
			const cTypeInfo& tTC1 = (const cTypeInfo&) typeid(cTestClass1);
			const cTypeInfo& tTC2 = (const cTypeInfo&) typeid(cTestClass2);
			UNITTEST2_TRUE(0 != tTC3.get_HashCode());

			const cTypeInfo& tRef0 = (const cTypeInfo&) typeid(cRefBase);
			UNITTEST2_TRUE(tRef0.get_HashCode() != tTC3.get_HashCode());

			const char* pszName2 = tTC3.get_Name();		// "class cUnitTest_cTypeInfo::cTestClass3"
			UNITTEST2_TRUE(pszName2 != nullptr);

			const cTypeInfo* ptTC1 = &tTC1;
			UNITTEST2_TRUE(ptTC1 != nullptr);
			const cTypeInfo* ptTC2 = &tTC2;
			UNITTEST2_TRUE(ptTC2 != nullptr);

			cTestClass3* pTC3 = &oTC3;
			UNITTEST2_TRUE(pTC3 != nullptr);
			cTestClass1* pTC1 = &oTC3;
			cTestClass2* pTC2 = &oTC3;
			UNITTEST2_TRUE((void*)pTC1 != (void*)pTC2);
			cTestClass1* pTC1b = DYNPTR_CAST(cTestClass1, pTC2);	// convert cTestClass2 back to peer cTestClass1. __RTDynamicCast
			UNITTEST2_TRUE(pTC1 == pTC1b);
		}

		UNITTEST2_METHOD(cTypeInfo)
		{
			// Enumerate methods from the vtable. __vfptr

			TestFloatTypes<float>();
			TestFloatTypes<double>();
			TestInt();
			TestRef();
			TestClass1();

			// Enumerate base classes and virtual methods from the vtable.
			// void* pVTable = pTC3->_vptr;
		}
	};
	UNITTEST2_REGISTER(cTypeInfo, UNITTEST_LEVEL_Core);
}
