//
//! @file cString.Tests.cpp
//
#include "pch.h"
#include "cString.h"

namespace Gray
{
	template< typename _TYPE_CH>
	void GRAYCALL UnitTestStrT() // static
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

#if 0 // ndef __linux__
		sTmp = THIS_t::GetT<int>(123);

		sTmp = THIS_t::GetT<UINT>(123);
#endif

		_TYPE_CH* pBuffer = sTmp.GetBuffer(0);
		sTmp.ReleaseBuffer(0);
		UNITTEST_TRUE(sTmp.GetLength() == 0);

		pBuffer = sTmp.GetBuffer(100);
		StrT::CopyLen<_TYPE_CH>(pBuffer, CSTRCONST("TEST"), 100);
		sTmp.ReleaseBuffer(k_StrLen_UNK);
		UNITTEST_TRUE(sTmp.GetLength() == 4);
	}

	UNITTEST_CLASS(cString)
	{
		UNITTEST_METHOD(cString)
		{
			const size_t nSizeStrA = sizeof(cStringA);	// 8
			STATIC_ASSERT(nSizeStrA == sizeof(void*), nSizeStrA);
			const size_t nSizeStrW = sizeof(cStringW);	// 8
			STATIC_ASSERT(nSizeStrA == nSizeStrW, cStringW);
			const size_t nSizeStrD = sizeof(CStringData); // 40
			STATIC_ASSERT(nSizeStrD > sizeof(void*) + sizeof(int), CStringData);

			UnitTestStrT<char>();
			UnitTestStrT<wchar_t>();
		}
	};

	UNITTEST2_REGISTER(cString, UNITTEST_LEVEL_Core);
}
