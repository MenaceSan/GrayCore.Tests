//! @file cList.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cList.h>
#include <GrayCore/include/cListNodeRef.h>
#include <GrayCore/include/cRandom.h>

namespace Gray {
struct cUnitTestListRef : public cListNodeRef<cUnitTestListRef> {
    int m_iVal;
    cUnitTestListRef(int iVal) : m_iVal(iVal) {}
};

struct UNITTEST_N(cList) : public cUnitTest {
    UNITTEST_METHOD(cList) {
        g_Rand.InitSeedOS();

        cListT<cUnitTestListRef> list;
        const int kCount = 1000;
        for (int i = 0; i < kCount; i++) {
            list.InsertHead(new cUnitTestListRef(i));
        }
        UNITTEST_TRUE(list.get_Count() == kCount);

        int count = kCount;
        int opCount = 0;
        // Randomly add and delete elements til empty.
        while (!list.isEmptyList()) {
            opCount++;
            // Count is accurate ?
            const cRandomBase::RAND_t nRand1 = g_Rand.GetRandUX(count);
            cUnitTestListRef* pRand = nullptr;
            int count2 = 0;
            cUnitTestListRef* pCur = list.get_Head();
            UNITTEST_TRUE(pCur);
            for (; pCur != nullptr; pCur = pCur->get_Next(), count2++) {
                if (nRand1 == count2) pRand = pCur;
            }
            UNITTEST_TRUE(list.get_Count() == count);
            UNITTEST_TRUE(count2 == count);
            UNITTEST_TRUE(pRand);
            UNITTEST_TRUE(pRand == list.GetAt(nRand1));

            const cRandomBase::RAND_t nRand2 = g_Rand.GetRandUX(4);
            if (nRand2) {
                // delete random element. higher probability.
                count--;
                pRand->DisposeThis();
            } else {
                // add new element
                count++;
                list.InsertListNode(new cUnitTestListRef(count), pRand);
            }
        }
        UNITTEST_TRUE(count == 0);
        UNITTEST_TRUE(list.isEmptyList());
        list.SetEmptyList();  // should already be empty!
    }
};
UNITTEST2_REGISTER(cList, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
