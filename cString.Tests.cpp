//
//! @file cString.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cString.h>

namespace Gray {
template <typename _TYPE_CH>
void GRAYCALL UnitTestStrT()  // static
{
    // k_asTextLines

    typedef cStringT<_TYPE_CH> THIS_t;

    THIS_t sTmp = StrT::Cast<_TYPE_CH>(CSTRCONST("sdfsdf"));
    UNITTEST_TRUE(sTmp.GetLength() == 6);

    _TYPE_CH ch = sTmp[3];
    UNITTEST_TRUE(ch == 's');
    UNITTEST_TRUE(sTmp[2] == 'f');

    StrLen_t iRetA = sTmp.Insert(0, CSTRCONST("ABC"));
    UNITTEST_TRUE(sTmp[2] == 'C');
    UNITTEST_TRUE(iRetA == 9);

#if 0  // ndef __linux__
		sTmp = THIS_t::GetT<int>(123);

		sTmp = THIS_t::GetT<UINT>(123);
#endif

    _TYPE_CH* pBuffer = sTmp.GetBuffer(0);
    sTmp.ReleaseBuffer(0);
    UNITTEST_TRUE(sTmp.GetLength() == 0);

    const StrLen_t lenRet = StrT::Copy<_TYPE_CH>(sTmp.GetSpanWrite(100), CSTRCONST("TEST"));
    UNITTEST_TRUE(lenRet == 4);
    sTmp.ReleaseBuffer(k_StrLen_UNK);
    UNITTEST_TRUE(sTmp.GetLength() == 4);
}

struct UNITTEST_N(cString) : public cUnitTest {
    UNITTEST_METHOD(cString) {
        // sizeof(cString) should be minimal. size of pointer.
        const size_t nSizeStrA = sizeof(cStringA);  // 8
        STATIC_ASSERT(nSizeStrA == sizeof(void*), nSizeStrA);
        const size_t nSizeStrW = sizeof(cStringW);        // 8
        STATIC_ASSERT(nSizeStrA == nSizeStrW, cStringW);  // same

        const size_t nSizeStrD = sizeof(cStringHeadT<char>);  // >= 40
        STATIC_ASSERT(nSizeStrD > sizeof(void*) + sizeof(int), cStringHeadT<char>);

        UnitTestStrT<char>();
        UnitTestStrT<wchar_t>();
    }
};
UNITTEST2_REGISTER(cString, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
