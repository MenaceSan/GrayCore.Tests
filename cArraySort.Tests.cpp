//! @file cArraySort.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cArraySort.h>
#include <GrayCore/include/cArrayT.h>
#include <GrayCore/include/cArrayUnique.h>
#include <GrayCore/include/cRandom.h>

namespace Gray {
class cUnitTestArraySort {
    // a Test class.
    //! Test sorted arrays. pick a simple template.
 public:
    int m_iSortVal;
    // HASHCODE_t _nHashCode;
    static int sm_nAllocated;

 public:
    cUnitTestArraySort(int iSortVal) : m_iSortVal(iSortVal) {
        sm_nAllocated++;
    }
    ~cUnitTestArraySort() {
        sm_nAllocated--;
    }
    int get_SortValue() const noexcept {
        return m_iSortVal;
    }
};

int cUnitTestArraySort::sm_nAllocated = 0;

struct UNITTEST_N(cArraySort) : public cUnitTest {
    template <class T>
    UINT TestArrayOrder(const T& a) {
        UINT sum = 0;
        UINT prev = 0;
        const UINT* pVals = a.get_PtrConst();
        for (int i = 0; i < a.GetSize(); i++) {
            UINT val = a[i];
            UNITTEST_TRUE(val > prev || (i == 0 && val == 0));
            prev = val;
            UNITTEST_TRUE(val < 10);
            sum += val;
        }
        return sum;
    }

    template <class T>
    void TestArraySet() {
        // for cArrayT or cArray
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

        UINT vals[] = {2, 3, 4};
        T aTest3;
        aTest3.InsertArray(0, TOSPAN(vals));
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

        const UINT* pVals = aTest2.get_PtrWork();
        for (int i = 0; i < 7; i++) {
            UNITTEST_TRUE(aTest2[i] == i);
        }

        aTest2.RemoveAt(3);
        UNITTEST_TRUE(TestArrayOrder(aTest2) == 18);
    }

    void PopRandom(cSpanX<UINT, UINT>& aVals) {
        for (int i = 0; i < aVals.GetSize(); i++) {
            aVals[i] = g_Rand.GetRandUX(0x10000);
        }
    }

    void TestArraySort() {
        // Test QSort. Create test array of random data then sort it.

        cArraySortVal<UINT> aVals;
        aVals.SetSize(300);
        PopRandom(aVals);
        UNITTEST_TRUE(aVals.GetSize() == 300);

        UNITTEST_TRUE(!aVals.isSpanSorted());
        aVals.QSort();
        UNITTEST_TRUE(aVals.isSpanSorted());

        // Test a list of cUniquePtr things sorted.
        UNITTEST_TRUE(cUnitTestArraySort::sm_nAllocated == 0);

        {
            cArraySortFacadeValue<cUniquePtr<cUnitTestArraySort>, cUnitTestArraySort*, int> aSortNew;
            aSortNew.AddSort(new cUnitTestArraySort(1), 0);
            aSortNew.AddSort(new cUnitTestArraySort(2), 0);
            aSortNew.AddSort(new cUnitTestArraySort(5), 0);
            aSortNew.AddSort(new cUnitTestArraySort(4), 0);
            aSortNew.AddSort(new cUnitTestArraySort(0), 0);
            aSortNew.AddSort(new cUnitTestArraySort(3), 0);

            UNITTEST_TRUE(aSortNew.isSpanSorted());
            UNITTEST_TRUE(aSortNew.GetSize() == 6);
            UNITTEST_TRUE(cUnitTestArraySort::sm_nAllocated == 6);
            UNITTEST_TRUE(aSortNew[3]->m_iSortVal == 3);
            UNITTEST_TRUE(aSortNew[4]->m_iSortVal == 4);

            bool bRet = aSortNew.RemoveKey(4);
            UNITTEST_TRUE(bRet);

            UNITTEST_TRUE(aSortNew.isSpanSorted());
            UNITTEST_TRUE(aSortNew.GetSize() == 5);
            UNITTEST_TRUE(cUnitTestArraySort::sm_nAllocated == 5);
            UNITTEST_TRUE(aSortNew[3]->m_iSortVal == 3);
            UNITTEST_TRUE(aSortNew[4]->m_iSortVal == 5);
        }

        UNITTEST_TRUE(cUnitTestArraySort::sm_nAllocated == 0);
    }

    void TestArrayIter() {
        // iterate the array.

        cArrayVal<UINT> aVals;
        aVals.SetSize(300);
        PopRandom(aVals);
        UNITTEST_TRUE(aVals.GetSize() == 300);

        UNITTEST_TRUE(!aVals.HasArg3(0x100001));    // will not have this.
        UNITTEST_TRUE(!aVals.HasArgN(0x100001));  // will not have this.

        const UINT uVal = aVals[150];

        UNITTEST_TRUE(aVals.HasArg3(uVal));  // will not have this.
        int i = aVals.FindIFor3(uVal);
        UNITTEST_TRUE(i <= 150);

        UNITTEST_TRUE(aVals.HasArgN(uVal));  // will not have this.
        i = aVals.FindIForN(uVal);
        UNITTEST_TRUE(i <= 150);

        i = 0;
        for (auto j : aVals) {
            UINT val0 = j;
            UINT val1 = aVals.GetAt(i);
            UINT val2 = aVals.ElementAt(i);
            UINT val3 = aVals[i];
            UNITTEST_TRUE(val0 == val1);
            UNITTEST_TRUE(val0 == val2);
            UNITTEST_TRUE(val0 == val3);
            i++;
        }
    }

    void TestArrayUnique() {
        cArrayUnique<cUnitTestArraySort> au;
        au.Add(new cUnitTestArraySort(1));
        au.Add(new cUnitTestArraySort(2));

        UNITTEST_TRUE(au.GetAtCheck(1) != nullptr);
        UNITTEST_TRUE(au.GetAtCheck(3) == nullptr);
    }

    UNITTEST_METHOD(cArraySort) {
        cArrayT<UINT> aTest1(1);
        STATIC_ASSERT(sizeof(aTest1) == sizeof(cRefPtr<int>), cArrayT);

        TestArraySet<cArrayT<UINT> >();
        TestArraySet<cArrayVal<UINT> >();
        TestArraySort();
        TestArrayIter();
        TestArrayUnique();
    }
};
UNITTEST2_REGISTER(cArraySort, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
