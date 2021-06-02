//
//! @file cArraySort.Tests.cpp
//
#include "pch.h"
#include "cArraySort.h"
#include "cArrayT.h"
#include "cRandom.h"
#include "cUniquePtr.h"

namespace Gray
{
	class cUnitTestArraySort
	{
		// a Test class.
		//! Test sorted arrays. pick a simple template.
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

	UNITTEST_CLASS(cArraySort)
	{
	public:

		template <class T>
		UINT TestArrayOrder(const T& a)
		{
			UINT sum = 0;
			UINT prev = 0;
			const UINT* pVals = a.GetData();
			for (int i = 0; i < a.GetSize(); i++)
			{
				UINT val = a[i];
				UNITTEST_TRUE(val > prev || (i == 0 && val == 0));
				prev = val;
				UNITTEST_TRUE(val < 10);
				sum += val;
			}
			return sum;
		}

		template <class T>
		void TestArraySet()
		{
			T aTest1(1);
			UNITTEST_TRUE(aTest1.GetSize() == 1);
			// TestArrayOrder(aTest1);		// un-init.

			T aTest2(2);
			aTest2.SetAt(0, 0);
			aTest2.SetAt(1, 6);
			UNITTEST_TRUE(aTest2.GetSize() == 2);
			UNITTEST_TRUE(aTest2[0] == 0);
			UNITTEST_TRUE(aTest2[1] == 6);
			TestArrayOrder(aTest2);

			UINT vals[] = { 2, 3, 4 };
			T aTest3;
			aTest3.InsertArray(0, vals, _countof(vals));
			UNITTEST_TRUE(aTest3.GetSize() == 3);
			UNITTEST_TRUE(aTest3[1] == 3);
			TestArrayOrder(aTest3);

			aTest3.InsertAt(0, 1);
			aTest3.InsertAt(4, 5);
			UNITTEST_TRUE(aTest3.GetSize() == 5);
			TestArrayOrder(aTest3);

			aTest2.InsertArray(1, aTest3);
			UNITTEST_TRUE(aTest2.GetSize() == 7);
			UNITTEST_TRUE(TestArrayOrder(aTest2) == 21);

			const UINT* pVals = aTest2.GetData();
			for (int i = 0; i < 7; i++)
			{
				UNITTEST_TRUE(aTest2[i] == i);
			}

			aTest2.RemoveAt(3);
			UNITTEST_TRUE(TestArrayOrder(aTest2) == 18);
		}

		void TestArraySort()
		{
			// Test QSort. Create test array of random data then sort it.

			cArrayVal<UINT> aVals;
			for (int i = 0; i < 300; i++)
			{
				aVals.Add(g_Rand.get_RandUns());
			}

			UNITTEST_TRUE(!aVals.isArraySorted());
			aVals.QSort();
			UNITTEST_TRUE(aVals.isArraySorted());

			// Test a list of cUniquePtr things sorted.
			UNITTEST_TRUE(cUnitTestArraySort::sm_nAllocated == 0);

			{
				cArraySortFacadeValue< cUniquePtr<cUnitTestArraySort>, cUnitTestArraySort*, int > aSortNew;
				aSortNew.Add(new cUnitTestArraySort(1));
				aSortNew.Add(new cUnitTestArraySort(2));
				aSortNew.Add(new cUnitTestArraySort(5));
				aSortNew.Add(new cUnitTestArraySort(4));
				aSortNew.Add(new cUnitTestArraySort(0));
				aSortNew.Add(new cUnitTestArraySort(3));

				UNITTEST_TRUE(aSortNew.isArraySorted());
				UNITTEST_TRUE(aSortNew.GetSize() == 6);
				UNITTEST_TRUE(cUnitTestArraySort::sm_nAllocated == 6);
				UNITTEST_TRUE(aSortNew[3]->m_iSortVal == 3);
				UNITTEST_TRUE(aSortNew[4]->m_iSortVal == 4);

				bool bRet = aSortNew.RemoveKey(4);
				UNITTEST_TRUE(bRet);

				UNITTEST_TRUE(aSortNew.isArraySorted());
				UNITTEST_TRUE(aSortNew.GetSize() == 5);
				UNITTEST_TRUE(cUnitTestArraySort::sm_nAllocated == 5);
				UNITTEST_TRUE(aSortNew[3]->m_iSortVal == 3);
				UNITTEST_TRUE(aSortNew[4]->m_iSortVal == 5);
			}

			UNITTEST_TRUE(cUnitTestArraySort::sm_nAllocated == 0);

		}

		UNITTEST_METHOD(cArraySort)
		{
			cArrayT<UINT> aTest1(1);
			STATIC_ASSERT(sizeof(aTest1) == sizeof(cRefPtr<int>), cArrayT);

			TestArraySet< cArrayT<UINT> >();
			TestArraySet< cArrayVal<UINT> >();
			TestArraySort();
		}
	};

	UNITTEST2_REGISTER(cArraySort, UNITTEST_LEVEL_Core);
}
