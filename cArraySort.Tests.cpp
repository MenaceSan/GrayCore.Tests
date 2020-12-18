//
//! @file cArraySort.Tests.cpp
//
#include "pch.h"
#include "cArraySort.h"
#include "cRandom.h"
#include "cNewPtr.h"

namespace Gray
{
	class cUnitTestArraySort
	{
	public:
		int m_iSortVal;
		// HASHCODE_t m_nHashCode;
		static int sm_nAllocated;
	public:
		cUnitTestArraySort(int iSortVal)
			: m_iSortVal(iSortVal)
		{
			sm_nAllocated++;
		}
		~cUnitTestArraySort()
		{
			sm_nAllocated--;
		}
		int get_SortValue() const noexcept
		{
			return m_iSortVal;
		}
	};

	int cUnitTestArraySort::sm_nAllocated = 0;

	UNITTEST2_CLASS(cArraySort)
	{ 
	public:

		UNITTEST2_METHOD(cArraySort)
		{
			//! Test sorted arrays. pick a simple template.

			// Test QSort. Create test array of random data then sort it.
			cArrayVal<UINT> aVals;
			for (int i = 0; i < 300; i++)
			{
				aVals.Add(g_Rand.get_RandUns());
			}

			UNITTEST2_TRUE(!aVals.isArraySorted());
			aVals.QSort();
			UNITTEST2_TRUE(aVals.isArraySorted());

			// Test a list of cNewPtr things sorted.
			UNITTEST2_TRUE(cUnitTestArraySort::sm_nAllocated == 0);
			{
				cArraySortFacadeValue< cNewPtr<cUnitTestArraySort>, cUnitTestArraySort*, int > aSortNew;
				aSortNew.Add(new cUnitTestArraySort(1));
				aSortNew.Add(new cUnitTestArraySort(2));
				aSortNew.Add(new cUnitTestArraySort(5));
				aSortNew.Add(new cUnitTestArraySort(4));
				aSortNew.Add(new cUnitTestArraySort(0));
				aSortNew.Add(new cUnitTestArraySort(3));

				UNITTEST2_TRUE(aSortNew.isArraySorted());
				UNITTEST2_TRUE(aSortNew.GetSize() == 6);
				UNITTEST2_TRUE(cUnitTestArraySort::sm_nAllocated == 6);
				UNITTEST2_TRUE(aSortNew[3]->m_iSortVal == 3);
				UNITTEST2_TRUE(aSortNew[4]->m_iSortVal == 4);

				bool bRet = aSortNew.RemoveKey(4);
				UNITTEST2_TRUE(bRet);

				UNITTEST2_TRUE(aSortNew.isArraySorted());
				UNITTEST2_TRUE(aSortNew.GetSize() == 5);
				UNITTEST2_TRUE(cUnitTestArraySort::sm_nAllocated == 5);
				UNITTEST2_TRUE(aSortNew[3]->m_iSortVal == 3);
				UNITTEST2_TRUE(aSortNew[4]->m_iSortVal == 5);
			}

			UNITTEST2_TRUE(cUnitTestArraySort::sm_nAllocated == 0);
		}
	};

	UNITTEST2_REGISTER(cArraySort, UNITTEST_LEVEL_Core);
}
