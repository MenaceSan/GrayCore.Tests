//
//! @file cList.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cList.h>
#include <GrayCore/include/cListNodeRef.h>

namespace Gray {
struct cUnitTestListRef : public cListNodeRef<cUnitTestListRef> {
    int m_iVal;
    cUnitTestListRef(int iVal) : m_iVal(iVal) {}
};

struct UNITTEST_N(cList) : public cUnitTest {
    UNITTEST_METHOD(cList) {
        cListT<cUnitTestListRef> list;
        // list.InsertHead(new cUnitTestListRef(1));
        // list.Empty();
    }
};
UNITTEST2_REGISTER(cList, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
