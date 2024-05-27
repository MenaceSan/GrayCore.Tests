//! @file cArraySortString.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cArraySortString.h>

namespace Gray {
struct UNITTEST_N(cArraySortString) : public cUnitTest {
    UNITTEST_METHOD(cArraySortString) {
        //! Test sorted arrays. pick a simple template.

        cArrayString<> arrayUns;
        UNITTEST_TRUE(arrayUns.GetSize() == 0);

        cArraySortStringA array1;
        for (ITERATE_t i = 0; i<_countof(k_asTextLines); i++) {
            array1.AddStr(cUnitTest::k_asTextLines[i]);
        }
        UNITTEST_TRUE(array1.GetSize() == _countof(k_asTextLines));

        StrLen_t iLength = 0;
        for (cString sVal : array1) {
            iLength += sVal.GetLength();
        }
        UNITTEST_TRUE(iLength >= 66);

        const cStringA* ppData = array1.get_PtrConst();
        UNITTEST_TRUE(ppData != nullptr);

        UNITTEST_TRUE(array1.isSpanSorted());
    }
};
UNITTEST2_REGISTER(cArraySortString, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
