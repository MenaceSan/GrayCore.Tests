//! @file cHeap.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cHeap.h>

namespace Gray {
__DECL_ALIGN(32) struct cTestHeapObject2 : public cHeapObject {
    DECLARE_HEAP_ALIGNED_ALLOC(cTestHeapObject2);  // replace new operator.
    DWORD a[128];                                  // add some size to allocate.
};
__DECL_ALIGN(64)
struct cTestHeapObject3 : public cHeapObject {  // default new operator.
    DWORD a[128];                               // add some size to allocate.
};

struct UNITTEST_N(cHeap) : public cUnitTest {
    void TestHeapObject() {
        auto* p1 = new cHeapObject();
        UNITTEST_TRUE(!cHeapAlign::IsHeapAlign(p1));
        auto* p2 = new cTestHeapObject2();
        UNITTEST_TRUE(cHeapAlign::IsHeapAlign(p2));

        const auto align1 = cHeap::GetAlign(p1);
        UNITTEST_TRUE(align1 >= cHeap::k_SizeAlignDef);
        const auto align2 = cHeap::GetAlign(p2);
        UNITTEST_TRUE(align2 >= 32);

        const auto size1a = cHeap::GetSize(p1);  // 16 for 64 bit.
        const auto size1b = cHeapAlign::GetSize(p1);
        UNITTEST_TRUE(size1a == size1b);

        const auto size2 = cHeapAlign::GetSize(p2);  // 167 in 64 bit
        UNITTEST_TRUE(size2 > size1a);

        delete p1;
        delete p2;

#if 0
			auto* p3 = new cTestHeapObject3();	// new op doesnt do align  !!! warning C4316: 'Gray::cTestHeapObject3': object allocated on the heap may not be aligned 64
			UNITTEST_TRUE(cHeapAlign::IsHeapAlign(p3));
			auto align3 = cHeap::GetAlign(p3);
			UNITTEST_TRUE(align3 >= 64);
			auto size3 = cHeapAlign::GetSize(p3);
			UNITTEST_TRUE(size3 > size1a);

			delete p3;
#endif

    }  // namespace Gray

    UNITTEST_METHOD(cHeap) {
        // test physical memory and the heap.
        cUnitTests& uts = cUnitTests::I();

        auto align1 = cHeap::GetAlign(CastNumToPtr(0x12331));
        UNITTEST_TRUE(align1 == 1);
        auto align16 = cHeap::GetAlign(CastNumToPtr(0x12350));
        UNITTEST_TRUE(align16 == 16);

        for (int i = 0; i < 10; i++) {
            TestHeapObject();
        }

        UNITTEST_TRUE(uts._pLog != nullptr);
        UNITTEST_TRUE(cHeap::Check());

        UINT64 uPhysTotal = cHeap::get_PhysTotal();
        UNITTEST_TRUE(uPhysTotal > 1024);
        UINT64 uPhysAvail = cHeap::get_PhysAvail();
        UNITTEST_TRUE(uPhysAvail > 1024);
        uts._pLog->addInfoF("Heap %s free of %s total", LOGSTR(cString::GetSizeK(uPhysAvail)), LOGSTR(cString::GetSizeK(uPhysTotal)));

        // Allocate a bunch of blocks and make sure they stay put til freed
        cBlob testBlob[32];
        for (ITERATE_t i = 0; i < (ITERATE_t)_countof(testBlob) && i < _countof(cUnitTests::k_asTextLines); i++) {
            testBlob[i].SetCopyAlloc(StrT::ToSpanStr<GChar_t>(k_asTextLines[i]));
            UNITTEST_TRUE(testBlob[i].isValidRead());
        }
        for (ITERATE_t j = 0; j < (ITERATE_t)_countof(testBlob) && j < _countof(cUnitTests::k_asTextLines); j++) {
            UNITTEST_TRUE(testBlob[j].isValidRead());
            const GChar_t* pszLine = k_asTextLines[j];
            UNITTEST_TRUE(testBlob[j].IsEqualSpan(StrT::ToSpanStr(pszLine)));
            testBlob[j].SetBlobNull();
        }

        UNITTEST_TRUE(cHeap::Check());

        // Test GetSize. NOTE: _MSC_VER always returns the exact size of alloc requested.
        for (size_t nSizeAlloc = 0; nSizeAlloc < 1024; nSizeAlloc++) {
            cBlob blob2(nSizeAlloc);
            size_t nSizeTest = blob2.get_AllocSize();
            if (nSizeAlloc == 0) {
                // Alloc 0 ? may return nullptr or not.
                uts._pLog->addInfoF("Heap alloc(%d) = ptr 0%x, size=%d", (int)nSizeAlloc, (int)CastPtrToNum(blob2.GetTPtrC()), (int)nSizeTest);
            }
            UNITTEST_TRUE(nSizeTest >= nSizeAlloc);
        }

        // NOT Aligned allocate. but always aligned to void* at least.
        for (int i = 1; i < 200; i++) {
            void* pData1N = cHeap::AllocPtr(i);
            cMem::Fill(pData1N, i, 0x11);
            UINT_PTR uValPtr = CastPtrToNum(pData1N);
            const auto alignRem = (uValPtr % (2 * _SIZEOF_PTR));
            UNITTEST_TRUE(alignRem == 0);                      // should always be true.
            UNITTEST_TRUE(!cHeapAlign::IsHeapAlign(pData1N));  // Should NOT report it is aligned.
            cHeap::FreePtr(pData1N);
        }

        // Aligned allocate.
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
        void* pData1A = cHeapAlign::AllocPtr(100, cHeap::k_SizeAlignDef * 2);
        cMem::Fill(pData1A, 100, 0x22);
        UNITTEST_TRUE(cHeapAlign::IsHeapAlign(pData1A));  // Should report it is aligned.
        cHeapAlign::FreePtr(pData1A);
#endif

        UNITTEST_TRUE(cHeap::Check());
    }
};
UNITTEST2_REGISTER(cHeap, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
