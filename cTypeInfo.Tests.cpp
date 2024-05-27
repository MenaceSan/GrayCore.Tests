//! @file cTypeInfo.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cFloat.h>
#include <GrayCore/include/cLogEvent.h>
#include <GrayCore/include/cTypeInfo.h>
#include <GrayCore/include/cTypes.h>

namespace Gray {
DECLARE_INTERFACE(ITestClass0) {
    IGNORE_WARN_INTERFACE(ITestClass0);
    virtual void f1() = 0;
};

struct UNITTEST_N(cTypeInfo) : public cUnitTest {
    class cTestClass1 : public ITestClass0 {
        virtual void f1() = 0;
    };
    class cTestClass2 : public ITestClass0 {
        virtual void f1() = 0;
        virtual void f2() = 0;
    };
    class cTestClass3 : public cTestClass2, public cTestClass1 {
        int _val;

        virtual void f1() {}
        virtual void f2() {}
        virtual void f3() {}
    };

    template <typename TYPE>
    void TestFloatTypes() {
        const TYPE kNaN = NAN;
        const TYPE kPInf = INFINITY;
        const TYPE kNInf = -INFINITY;

        UNITTEST_TRUE(cFloat::IsFinite<TYPE>(0));
        UNITTEST_TRUE(cFloat::IsFinite<TYPE>(123));
        UNITTEST_TRUE(cFloat::IsFinite<TYPE>((TYPE)123.456));
        UNITTEST_TRUE(!cFloat::IsFinite<TYPE>(kNaN));
        UNITTEST_TRUE(!cFloat::IsFinite<TYPE>(kPInf));
        UNITTEST_TRUE(!cFloat::IsFinite<TYPE>(kNInf));

        UNITTEST_TRUE(!cFloat::IsNaN<TYPE>(0));
        UNITTEST_TRUE(!cFloat::IsNaN<TYPE>(123));
        UNITTEST_TRUE(!cFloat::IsNaN<TYPE>((TYPE)123.456));
        UNITTEST_TRUE(cFloat::IsNaN<TYPE>(kNaN));
        UNITTEST_TRUE(!cFloat::IsNaN<TYPE>(kPInf));
        UNITTEST_TRUE(!cFloat::IsNaN<TYPE>(kNInf));

        UNITTEST_TRUE(!cFloat::IsInfinite<TYPE>(0));
        UNITTEST_TRUE(!cFloat::IsInfinite<TYPE>(123));
        UNITTEST_TRUE(!cFloat::IsInfinite<TYPE>((TYPE)123.456));
        UNITTEST_TRUE(!cFloat::IsInfinite<TYPE>(kNaN));
        UNITTEST_TRUE(cFloat::IsInfinite<TYPE>(kPInf));
        UNITTEST_TRUE(cFloat::IsInfinite<TYPE>(kNInf));
    }

    void TestInt() {
        int nInt = 0;
        const TYPEINFO_t& tInt0 = typeid(int);
        const TYPEINFO_t& tInt1 = typeid(nInt);
        // const TYPEINFO_t& tInt2 = typeid( decltype(nInt));		// Test use of C++11 feature "-Wc++0x-compat"
        UNITTEST_TRUE(tInt0 == tInt1);
        // UNITTEST_TRUE(tInt0 == tInt2);
        UNITTEST_TRUE(nInt == 0);  // just for a ref.
    }

    void TestRef() {
        const cTypeInfo& tRef0 = (const cTypeInfo&)typeid(cLogEvent);

        cLogEvent oSmart(LOG_ATTR_0, LOGLVL_t::_ANY, "TEST");  // no refs.
        const cTypeInfo& tRef1 = (const cTypeInfo&)typeid(oSmart);

        const char* pszName0 = tRef0.get_Name();  // "class Gray::cRefBase"
        const char* pszName1 = tRef1.get_Name();  // "class Gray::cRefBase"
        UNITTEST_TRUE(pszName1 != nullptr);

        size_t h0 = tRef0.get_HashCode();
        UNITTEST_TRUE(h0 != 0);

        size_t h1 = tRef1.get_HashCode();
        UNITTEST_TRUE(h0 == h1);  // same type

#ifdef _MSC_VER
        const char* pszRawName1 = tRef1.raw_name();  // Get M$ Mangled/Raw name ".?AVcRefBase@Gray@@"
        UNITTEST_TRUE(pszRawName1 != nullptr);
#endif
    }

    void TestClass1() {
        // compile time type info. typeid()
        cTestClass3 oTC3;
        const cTypeInfo& tTC3 = GETTYPEINFO(oTC3);
        const cTypeInfo& tTC1 = GETTYPEINFO(cTestClass1);
        const cTypeInfo& tTC2 = GETTYPEINFO(cTestClass2);
        UNITTEST_TRUE(0 != tTC3.get_HashCode());

        const cTypeInfo& tRef0 = GETTYPEINFO(cRefBase);
        UNITTEST_TRUE(tRef0.get_HashCode() != tTC3.get_HashCode());

        const char* pszName2 = tTC3.get_Name();  // "class cUnitTest_cTypeInfo::cTestClass3"
        UNITTEST_TRUE(pszName2 != nullptr);

        const cTypeInfo* ptTC1 = &tTC1;
        UNITTEST_TRUE(ptTC1 != nullptr);
        const cTypeInfo* ptTC2 = &tTC2;
        UNITTEST_TRUE(ptTC2 != nullptr);

        cTestClass3* pTC3 = &oTC3;
        UNITTEST_TRUE(pTC3 != nullptr);
        cTestClass1* pTC1 = &oTC3;
        cTestClass2* pTC2 = &oTC3;
        UNITTEST_TRUE((void*)pTC1 != (void*)pTC2);
        cTestClass1* pTC1b = DYNPTR_CAST(cTestClass1, pTC2);  // convert cTestClass2 back to peer cTestClass1. __RTDynamicCast
        UNITTEST_TRUE(pTC1 == pTC1b);

        // Type should still be detected as cTestClass3

        // Enumerate base classes and virtual methods from the vtable. vtable. __vfptr
        // void* pVTable = oTC3._val;
    }

    UNITTEST_METHOD(cTypeInfo) {
        // Enumerate methods from the typeid()

        TestFloatTypes<float>();
        TestFloatTypes<double>();
        TestInt();
        TestRef();
        TestClass1();

        std::underlying_type<CTYPE_FLAG_TYPE_>::type abyte = 123;
    }
};
UNITTEST2_REGISTER(cTypeInfo, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
