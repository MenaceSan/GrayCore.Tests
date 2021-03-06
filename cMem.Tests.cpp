// 
//! @file cMem.Tests.cpp
//
#include "pch.h"
#include "cMem.h"

namespace Gray
{
	template <class TYPE>
	void UnitTestMem(const TYPE nValH)
	{
		TYPE nValRev = nValH;
		cValArray::ReverseArrayBlocks(&nValRev, sizeof(nValRev), 1);

		TYPE nValRev2 = nValH;
		cValArray::ReverseArray<BYTE>((BYTE*)&nValRev2, sizeof(nValRev2));
		UNITTEST_TRUE(nValRev2 == nValRev);

		TYPE nValRev3 = cMemT::ReverseType(nValH);
		UNITTEST_TRUE(nValRev3 == nValRev);

		TYPE nValN = cMemT::HtoN(nValH);
#ifdef USE_LITTLE_ENDIAN
		// Bytes must be reversed.
		UNITTEST_TRUE(nValN == nValRev);
#else
		UNITTEST_TRUE(nValN == nValH);	// no change
#endif
		UNITTEST_TRUE(cMemT::NtoH(nValN) == nValH);	// back to original value.
	}

	UNITTEST_CLASS(cMem)
	{
		UNITTEST_METHOD(cMem)
		{
			// IsValid
			static const int k_Val = 123;	// i should not be able to write to this !?
			UNITTEST_TRUE(!cMem::IsCorrupt(&k_Val, 1, false));	// read static/const memory is valid.

			// Write to nullptr and low memory?
			UNITTEST_TRUE(cMem::IsCorrupt(nullptr, 1));
			UNITTEST_TRUE(cMem::IsCorrupt((void*)12, 1, true));

#if 0 // This doesn't work well in VS2015 in x64 for some reason. Does not continue from exception.
			// Write to const/ROM space?
			bool bCanWriteToROM = !cMem::IsCorrupt(&k_Val, 1, true);
			UNITTEST_TRUE(!bCanWriteToROM);	// writing to const should NOT be valid !?

			// Write to Code?
			bool bCanWriteToCode = !cMem::IsCorrupt((void*)&cMem::CompareIndex, 1, true);
			UNITTEST_TRUE(!bCanWriteToCode);
#endif

			// CompareIndex
			BYTE szTmp1[32];
			BYTE szTmp2[32];
			cMem::Fill(szTmp1, sizeof(szTmp1), 1);
			cMem::Fill(szTmp2, sizeof(szTmp2), 2);
			size_t nRet = cMem::CompareIndex(szTmp1, szTmp2, 4);
			UNITTEST_TRUE(nRet == 0);

			szTmp1[0] = 2;
			nRet = cMem::CompareIndex(szTmp1, szTmp2, 8);
			UNITTEST_TRUE(nRet == 1);

			nRet = cMem::CompareIndex(szTmp1, szTmp2, 9);
			UNITTEST_TRUE(nRet == 1);

			cMem::Copy(szTmp1, szTmp2, sizeof(szTmp2));
			nRet = cMem::CompareIndex(szTmp1, szTmp2, 9);
			UNITTEST_TRUE(nRet == 9);

			// Host order test data.
			UnitTestMem<WORD>(0x1234);
			UnitTestMem<UINT32>(0x12345678);
			UnitTestMem<UINT64>(0x123456789abcdef0ULL);
			UnitTestMem<ULONG>(0x12345678);	// Maybe 32 or 64 bit ?

			char szTmp[k_TEXTBLOB_LEN * 4 + 10];
			StrLen_t nLen = cMem::ConvertToString(szTmp, STRMAX(szTmp), (const BYTE*)(const char*)k_sTextBlob.m_A, k_TEXTBLOB_LEN);
			UNITTEST_TRUE(nLen >= k_TEXTBLOB_LEN);

			BYTE bTmp[k_TEXTBLOB_LEN + 10];
			size_t nSizeRet = cMem::ReadFromString(bTmp, STRMAX(bTmp), szTmp);
			UNITTEST_TRUE(nSizeRet == (size_t)k_TEXTBLOB_LEN);
			UNITTEST_TRUE(cMem::IsEqual(bTmp, (const char*)k_sTextBlob.m_A, nSizeRet));
			UNITTEST_TRUE(!cMem::IsCorrupt(szTmp, sizeof(szTmp)));
			UNITTEST_TRUE(!cMem::IsCorrupt(bTmp, sizeof(bTmp)));
		}
	};
	UNITTEST2_REGISTER(cMem, UNITTEST_LEVEL_Core);
}
