// 
//! @file cHeap.Tests.cpp
//
#include "pch.h"
#include "cHeap.h"

namespace Gray
{
	UNITTEST_CLASS(cHeap)
	{
		UNITTEST_METHOD(cHeap)
		{
			// test physical memory and the heap.
			cUnitTests& uts = cUnitTests::I();

			UNITTEST_TRUE(uts.m_pLog != nullptr);
			UNITTEST_TRUE(cHeap::Check());

			UINT64 uPhysTotal = cHeap::get_PhysTotal();
			UNITTEST_TRUE(uPhysTotal);
			UINT64 uPhysAvail = cHeap::get_PhysAvail();
			UNITTEST_TRUE(uPhysAvail);
			uts.m_pLog->addInfoF("Heap %s free of %s total", LOGSTR(cString::GetSizeK(uPhysAvail)), LOGSTR(cString::GetSizeK(uPhysTotal)));

			// Allocate a bunch of blocks and make sure they stay put til freed
			cHeapBlock test[32];
			for (ITERATE_t i = 0; i < (ITERATE_t)_countof(test) && !k_asTextLines[i].isNull(); i++)
			{
				const GChar_t* pszLine = k_asTextLines[i];
				StrLen_t iLen = StrT::Len(pszLine);
				test[i].Alloc(pszLine, iLen);
				UNITTEST_TRUE(test[i].isValidRead());
			}
			for (ITERATE_t j = 0; j < (ITERATE_t)_countof(test) && !k_asTextLines[j].isNull(); j++)
			{
				UNITTEST_TRUE(test[j].isValidRead());
				const GChar_t* pszLine = k_asTextLines[j];
				StrLen_t iLen = StrT::Len(pszLine);
				UNITTEST_TRUE(cMem::IsEqual(pszLine, test[j].get_DataBytes(), iLen));
				test[j].Free();
			}

			UNITTEST_TRUE(cHeap::Check());

			// Test GetSize. NOTE: _MSC_VER always returns the exact size of alloc requested.
			for (size_t nSizeAlloc = 0; nSizeAlloc < 1024; nSizeAlloc++)
			{
				cHeapBlock heapblock(nSizeAlloc);
				size_t nSizeTest = heapblock.get_AllocSize();
				if (nSizeAlloc == 0)
				{
					// Alloc 0 ? may return nullptr or not.
					uts.m_pLog->addInfoF("Heap alloc(%d) = ptr 0%x, size=%d", (int)nSizeAlloc, (int)(INT_PTR)heapblock.get_DataBytes(), (int)nSizeTest);
				}
				UNITTEST_TRUE(nSizeTest >= nSizeAlloc);
			}

			// NOT Aligned allocate.
			void* pData1N = cHeap::AllocPtr(100);
			cValArray::FillSize<BYTE>(pData1N, 100, 0x11);
			UNITTEST_TRUE(!cHeapAlign::IsAlignedAlloc(pData1N, 16)); // Should NOT report it is aligned.
			cHeap::FreePtr(pData1N);

			// Aligned allocate.
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
			void* pData1A = cHeapAlign::AllocPtr(100, 16);
			cValArray::FillSize<BYTE>(pData1A, 100, 0x22);
			UNITTEST_TRUE(cHeapAlign::IsAlignedAlloc(pData1A, 16)); // Should report it is aligned.
			cHeapAlign::FreePtr(pData1A);
#endif

			UNITTEST_TRUE(cHeap::Check());
		}
	};
	UNITTEST2_REGISTER(cHeap, UNITTEST_LEVEL_Core);
}
