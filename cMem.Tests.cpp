//! @file cMem.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cMem.h>

namespace Gray {
template <class TYPE>
void UnitTestMem(const TYPE nValH) {
    TYPE nValRev = nValH;
    cValSpan::ReverseArrayBlocks(&nValRev, sizeof(nValRev), 1);

    TYPE nValRev2 = nValH;
    cValSpan::ReverseArray<BYTE>((BYTE*)&nValRev2, sizeof(nValRev2));
    UNITTEST_TRUE(nValRev2 == nValRev);

    TYPE nValRev3 = cMemT::ReverseType(nValH);
    UNITTEST_TRUE(nValRev3 == nValRev);

    TYPE nValN = cMemT::HtoN(nValH);
#ifdef USE_LITTLE_ENDIAN
    // Bytes must be reversed.
    UNITTEST_TRUE(nValN == nValRev);
#else
    UNITTEST_TRUE(nValN == nValH);  // no change
#endif
    UNITTEST_TRUE(cMemT::NtoH(nValN) == nValH);  // back to original value.
}

struct UNITTEST_N(cMem) : public cUnitTest {

    void TestSpan() {
        cSpanStatic<128> memSpanStatic;
        STATIC_ASSERT(sizeof(memSpanStatic)  == 128, cSpanStatic);
        static const size_t k_SizeStatic = sizeof(memSpanStatic);   
        UNITTEST_TRUE(k_SizeStatic == 128);

        wchar_t tmp2[123];
        STATIC_ASSERT(_countof(tmp2) == 123, tmp2);
        STATIC_ASSERT(sizeof(tmp2) == 123 * 2, tmp2);

        const cSpan<wchar_t> span2a(tmp2, _countof(tmp2));  // same as TOSPAN()?. keep this!
        UNITTEST_TRUE(span2a.get_Count() == 123 );
        UNITTEST_TRUE(span2a.get_DataSize() == 123 * 2 );

        auto span2b(TOSPAN(tmp2));
        UNITTEST_TRUE(span2b.get_Count() == 123);
        UNITTEST_TRUE(span2b.get_DataSize() == 123 * 2);

        wchar_t* ppCmds[128];
        auto span3(TOSPAN(ppCmds));
        UNITTEST_TRUE(span3.get_Count() == 128);
        UNITTEST_TRUE(span3.get_DataSize() == 128 * sizeof(wchar_t*));

    }

    UNITTEST_METHOD(cMem) {

        // IsValid
        static const int k_Val = 123;                          // i should not be able to write to this !?
        UNITTEST_TRUE(!cMem::IsCorruptApp(&k_Val, 1, false));  // read static/const memory is valid.

        // Write to nullptr and low memory?
        UNITTEST_TRUE(cMem::IsCorruptApp(nullptr, 1));
        UNITTEST_TRUE(cMem::IsCorruptApp((void*)12, 1, true));

#if 0  // This doesn't work well in VS2015 in x64 for some reason. Does not continue from exception.
       // Write to const/ROM space?
			bool bCanWriteToROM = !cMem::IsCorruptApp(&k_Val, 1, true);
			UNITTEST_TRUE(!bCanWriteToROM);	// writing to const should NOT be valid !?

			// Write to Code?
			bool bCanWriteToCode = !cMem::IsCorruptApp((void*)&cMem::CompareIndex, 1, true);
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
        UnitTestMem<ULONG>(0x12345678);  // Maybe 32 or 64 bit ?

        char szTmp[k_TEXTBLOB_LEN * 4];
        StrLen_t nLen = StrT::ConvertToCSV(TOSPAN(szTmp), ToSpan<char>(k_sTextBlob));
        UNITTEST_TRUE(nLen >= k_sTextBlob._Len); // 2087 / 566

        BYTE bTmp[k_TEXTBLOB_LEN + 10];
        size_t nSizeRet = TOSPAN(bTmp).ReadFromCSV(szTmp);
        UNITTEST_TRUE(nSizeRet == (size_t)k_sTextBlob._Len);
        UNITTEST_TRUE(ToSpan<char>(k_sTextBlob).IsEqualData(bTmp));
        UNITTEST_TRUE(!cMem::IsCorruptApp(szTmp, sizeof(szTmp)));
        UNITTEST_TRUE(!cMem::IsCorruptApp(bTmp, sizeof(bTmp)));
    }
};
UNITTEST2_REGISTER(cMem, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
