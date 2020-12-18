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
		UNITTEST2_TRUE(sTmp.GetLength() == 6);

		_TYPE_CH ch = sTmp[3];
		UNITTEST2_TRUE(ch == 's');
		UNITTEST2_TRUE(sTmp[2] == 'f');

		StrLen_t iRetA = sTmp.Insert(0, CSTRCONST("ABC"));
		UNITTEST2_TRUE(sTmp[2] == 'C');
		UNITTEST2_TRUE(iRetA == 9);

#if 0 // ndef __linux__
		sTmp = THIS_t::GetT<int>(123);

		sTmp = THIS_t::GetT<UINT>(123);
#endif

		_TYPE_CH* pBuffer = sTmp.GetBuffer(0);
		sTmp.ReleaseBuffer(0);
		UNITTEST2_TRUE(sTmp.GetLength() == 0);

		pBuffer = sTmp.GetBuffer(100);
		StrT::CopyLen<_TYPE_CH>(pBuffer, CSTRCONST("TEST"), 100);
		sTmp.ReleaseBuffer(k_StrLen_UNK);
		UNITTEST2_TRUE(sTmp.GetLength() == 4);
	}

	UNITTEST2_CLASS(cString)
	{
		UNITTEST2_METHOD(cString)
		{
			UnitTestStrT<char>();
			UnitTestStrT<wchar_t>();
		}
	};

	UNITTEST2_REGISTER(cString, UNITTEST_LEVEL_Core);
}
