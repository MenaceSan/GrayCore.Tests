//
//! @file cWinHeap.Tests.cpp
//
#include "pch.h"
#include "cWinHeap.h"

#if defined(_WIN32) && ! defined(UNDER_CE)

namespace Gray
{
	UNITTEST_CLASS(cWinHeap)
	{
		UNITTEST_METHOD(cWinHeap)
		{
			// Enum all the private heaps for this process.
			cUnitTests& uts = cUnitTests::I();

			HANDLE aProcessHeaps[256];
			DWORD dwRet = ::GetProcessHeaps(_countof(aProcessHeaps), aProcessHeaps);
			UNITTEST_TRUE(dwRet > 0);

			for (DWORD i = 0; i < dwRet; i++)
			{
				cWinHeap heap(aProcessHeaps[i], false);
				if (!heap.isValidHeap())
				{
					uts.m_pLog->addInfoF("cWinHeap 0%x Not valid", heap.get_Handle());
					continue;
				}

				ULONG HeapInformation = 0;
				SIZE_T nSizeData = sizeof(HeapInformation);
				// HEAP_INFORMATION_CLASS = 1 = HeapEnableTerminationOnCorruption
				heap.QueryInformation((HEAP_INFORMATION_CLASS)1, &HeapInformation, &nSizeData);

				uts.m_pLog->addInfoF("cWinHeap 0%x compact=%d, term=%d", heap.get_Handle(), heap.Compact(), HeapInformation);
			}
		}
	};
	UNITTEST2_REGISTER(cWinHeap, UNITTEST_LEVEL_Core);
}
#endif
