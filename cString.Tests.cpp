//! @file cString.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cString.h>

namespace Gray {

template <typename _TYPE_CH>
void GRAYCALL TestStrChange() {  // static
    typedef cStringT<_TYPE_CH> THIS_t;

    THIS_t a("abc");
    THIS_t b = a;
    UNITTEST_TRUE(a == b);
    UNITTEST_TRUE(a.c_str() == b.c_str());

    a.SetAt(1,'B');
    UNITTEST_TRUE(a != b);
    UNITTEST_TRUE(a.c_str() != b.c_str());
    UNITTEST_TRUE(a == CSTRCONST("aBc"));

    a.SetAt(1, 'b');
    UNITTEST_TRUE(a == b);
    UNITTEST_TRUE(a.c_str() != b.c_str());
    UNITTEST_TRUE(a == CSTRCONST("abc"));
}

template <typename _TYPE_CH>
void GRAYCALL TestStrT() {  // static
    // k_asTextLines
    typedef cStringT<_TYPE_CH> THIS_t;

    TestStrChange<_TYPE_CH>();

    THIS_t sTmp = StrT::Cast<_TYPE_CH>(CSTRCONST("sdfsdf"));
    UNITTEST_TRUE(sTmp.GetLength() == 6);

    _TYPE_CH ch = sTmp[3];
    UNITTEST_TRUE(ch == 's');
    UNITTEST_TRUE(sTmp[2] == 'f');

    const StrLen_t iRetA = sTmp.InsertSpan(0, ToSpanStr<_TYPE_CH>(CSTRCONST("ABC")));
    UNITTEST_TRUE(sTmp[2] == 'C');
    UNITTEST_TRUE(iRetA == 9);

#if 0  // ndef __linux__
		sTmp = THIS_t::GetT<int>(123);

		sTmp = THIS_t::GetT<UINT>(123);
#endif

    _TYPE_CH* pBuffer = sTmp.GetBuffer(0);
    sTmp.ReleaseBuffer(0);
    UNITTEST_TRUE(sTmp.GetLength() == 0);
    UNITTEST_TRUE(sTmp.IsEmpty());

    const StrLen_t lenRet = StrT::CopyPtr<_TYPE_CH>(sTmp.GetSpanWrite(100), CSTRCONST("TEST"));
    UNITTEST_TRUE(lenRet == 4);
    sTmp.ReleaseBuffer(k_StrLen_UNK);
    UNITTEST_TRUE(sTmp.GetLength() == 4);
}

struct UNITTEST_N(cString) : public cUnitTest {
    UNITTEST_METHOD(cString) {
        // sizeof(cString) should be minimal. size of pointer.
        const size_t nSizeStrA = sizeof(cStringA);  // 8
        STATIC_ASSERT(nSizeStrA == _SIZEOF_PTR, nSizeStrA);
        const size_t nSizeStrW = sizeof(cStringW);        // 8
        STATIC_ASSERT(nSizeStrA == nSizeStrW, cStringW);  // same

        const size_t nSizeStrD = sizeof(cStringHeadT<char>);  // >= 40
        STATIC_ASSERT(nSizeStrD > _SIZEOF_PTR + sizeof(int), cStringHeadT<char>);

        TestStrT<char>();
        TestStrT<wchar_t>();
    }
};
UNITTEST2_REGISTER(cString, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
