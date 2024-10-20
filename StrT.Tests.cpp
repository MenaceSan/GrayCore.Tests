//! @file StrT.Tests.cpp

#include "pch.h"
#include <GrayCore/include/StrT.h>
#include <GrayCore/include/StrU.h>
#include <GrayCore/include/cSpan.h>

namespace Gray {
static const cStrConst k_t1 = CSTRCONST("sdfsdf1");
static const cStrConst k_t2 = CSTRCONST("sdfsdF23 5");  // lower case = higher number ASCII.
static const cStrConst k_t3 = CSTRCONST("a.b.c");

struct UNITTEST_N(StrT) : public cUnitTest {
    template <typename TYPE>
    static void UnitTestBasic() {  // static

        // Basic tests first.

        const cSpan<TYPE> sp0 = StrT::ToSpanStr<TYPE>(nullptr);
        UNITTEST_TRUE(sp0.get_Count() == 0);
        const cSpan<TYPE> sp1 = StrT::ToSpanStr<TYPE>(CSTRCONST(""));
        UNITTEST_TRUE(sp1.get_Count() == 0);

        bool bPrintable = StrT::IsPrintable<TYPE>(k_t3, 2);
        UNITTEST_TRUE(bPrintable);
        bPrintable = StrT::IsPrintable("a\177", 2);
        UNITTEST_TRUE(!bPrintable);

        bool bIsWhitespace = StrT::IsWhitespace<TYPE>(CSTRCONST("  \f\r\n\t "), k_StrLen_UNK);
        UNITTEST_TRUE(bIsWhitespace);
        bIsWhitespace = StrT::IsWhitespace<TYPE>(nullptr);
        UNITTEST_TRUE(bIsWhitespace);
        bIsWhitespace = StrT::IsWhitespace<TYPE>(cStrConst::k_Empty);
        UNITTEST_TRUE(bIsWhitespace);

        const StrLen_t nLen = StrT::Len<TYPE>(k_t1);
        UNITTEST_TRUE(nLen == 7);

        COMPARE_t eComp = StrT::Cmp<TYPE>(k_t1, k_t2);
        UNITTEST_TRUE(eComp > 0);

        eComp = StrT::CmpI<TYPE>(k_t1, k_t2);
        UNITTEST_TRUE(eComp < 0);

        eComp = StrT::CmpN<TYPE>(k_t1, k_t2, 16);
        UNITTEST_TRUE(eComp > 0);

        eComp = StrT::CmpIN<TYPE>(k_t1, k_t2, 16);
        UNITTEST_TRUE(eComp < 0);

        bool bCmp = StrT::StartsWithI<TYPE>(k_t2, k_t1);
        UNITTEST_TRUE(!bCmp);

        bCmp = StrT::EndsWithI<TYPE>(ToSpanStr<TYPE>(k_t2), ToSpanStr<TYPE>(k_t1));
        UNITTEST_TRUE(!bCmp);

        HASHCODE32_t nHashCode = StrT::GetHashCode32(ToSpanStr<TYPE>(k_t1));
        UNITTEST_TRUE(nHashCode == 0x1488c5b4);
    }

    template <typename TYPE>
    static void UnitTestFind() {  // static

        static const cStrConst k_tSent = CSTRCONST("This is a sentence. And another. // comment");

        const TYPE* pRetChar = StrT::GetNonWhitespace<TYPE>(k_tSent);
        UNITTEST_TRUE(pRetChar != nullptr);

        // char in string.
        pRetChar = StrT::FindChar<TYPE>(k_t1, (TYPE)'f');
        UNITTEST_TRUE(pRetChar != nullptr && *pRetChar == 'f');
        pRetChar = StrT::FindChar<TYPE>(k_t1, (TYPE)'Z');
        UNITTEST_TRUE(pRetChar == nullptr);

        StrLen_t nLen = StrT::FindCharN<TYPE>(k_t1, (TYPE)'f');
        UNITTEST_TRUE(nLen == 2);
        nLen = StrT::FindCharN<TYPE>(k_t1, (TYPE)'Z');
        UNITTEST_TRUE(nLen == -1);

        pRetChar = StrT::FindCharRev(ToSpanStr<TYPE>(k_t1), (TYPE)'f');
        UNITTEST_TRUE(pRetChar != nullptr && *pRetChar == 'f');  // f1
        pRetChar = StrT::FindCharRev(ToSpanStr<TYPE>(k_t1), (TYPE)'Z');
        UNITTEST_TRUE(pRetChar == nullptr);

        pRetChar = StrT::FindTokens<TYPE>(ToSpanStr<TYPE>(k_tSent), k_t2);
        UNITTEST_TRUE(pRetChar != nullptr);
        pRetChar = StrT::FindTokens<TYPE>(ToSpanStr<TYPE>(k_tSent), CSTRCONST("yz"));
        UNITTEST_TRUE(pRetChar == nullptr);

        // strings in string.

        const TYPE* pszTest = CSTRCONST("abcdefabcdefg");
        const TYPE* pszRet = StrT::FindStr<TYPE>(pszTest, CSTRCONST("abcdefg"));
        UNITTEST_TRUE(pszRet == pszTest + 6);

        pszTest = CSTRCONST("abcabcabcabc");
        pszRet = StrT::FindStr<TYPE>(pszTest, CSTRCONST("abca"));
        UNITTEST_TRUE(pszRet == pszTest + 0);
        pszRet = StrT::FindStr<TYPE>(pszTest, CSTRCONST("zzzz"));
        UNITTEST_TRUE(pszRet == nullptr);

        pszRet = StrT::FindStrI<TYPE>(pszTest, CSTRCONST("AbCa"));
        UNITTEST_TRUE(pszRet == pszTest + 0);
        pszRet = StrT::FindStrI<TYPE>(pszTest, CSTRCONST("zzzz"));
        UNITTEST_TRUE(pszRet == nullptr);

        nLen = StrT::FindWord<TYPE>(k_tSent, CSTRCONST("sdfsdf"), k_tSent._Len);
        UNITTEST_TRUE(nLen == -1);
        nLen = StrT::FindWord<TYPE>(k_tSent, CSTRCONST("sentence"), k_tSent._Len);
        UNITTEST_TRUE(nLen == 10);
    }

    template <typename TYPE>
    static void UnitTestInt() {  // static

        UINT64 ulVal = StrT::toUL<TYPE>(CSTRCONST("0xFFFFFFFF"), nullptr, 8);  // even if radix base is wrong. encoding should work.
        UNITTEST_TRUE(ulVal == 0xFFFFFFFF);

        ulVal = StrT::toUL<TYPE>(CSTRCONST("0xFFFFFFFF"), nullptr);
        UNITTEST_TRUE(ulVal == 0xFFFFFFFF);

        ulVal = StrT::toUL<TYPE>(CSTRCONST("FFFFFFFF"), nullptr);  // radix wrong.
        UNITTEST_TRUE(ulVal == 0);

        ulVal = StrT::toUL<TYPE>(CSTRCONST("FFFFFFFF"), nullptr, 16);
        UNITTEST_TRUE(ulVal == 0xFFFFFFFF);

        INT64 iVal64 = StrT::toIL<TYPE>(CSTRCONST("1234567"), nullptr, 10);
        UNITTEST_TRUE(iVal64 == 1234567);

        int iVal = StrT::toI<TYPE>(CSTRCONST("-123"));
        UNITTEST_TRUE(iVal == -123);

        iVal = StrT::toI<TYPE>(CSTRCONST("123"));
        UNITTEST_TRUE(iVal == 123);

        iVal = StrT::toI<TYPE>(CSTRCONST("0x123"));
        UNITTEST_TRUE(iVal == 0x123);

        TYPE szTmp[StrNum::k_LEN_MAX_DIGITS_INT + 32];
        StrLen_t nLenRet = StrT::ILtoA<TYPE>(123123, TOSPAN(szTmp), 10);
        UNITTEST_TRUE(!StrT::Cmp<TYPE>(szTmp, CSTRCONST("123123")));

        nLenRet = StrT::ULtoAK<TYPE>(123123, TOSPAN(szTmp), 1024, true);
        UNITTEST_TRUE(nLenRet == 8);
    }

    template <typename TYPE>
    static void UnitTestDouble() {  // static

        // Test float,double.  Test MUST be reversible.
        TYPE szTmp[StrNum::k_LEN_MAX_DIGITS + 32];

        const cStrConst kValS = CSTRCONST("123.123");
        const double kValN = 123.123;

        double dVal = StrT::toDouble<TYPE>(kValS, nullptr);
        UNITTEST_TRUE(dVal == kValN);  // NOT 123.12299999

        StrLen_t nLenRet = StrT::DtoA<TYPE>(dVal, TOSPAN(szTmp));
        UNITTEST_TRUE(!StrT::Cmp<TYPE>(szTmp, kValS));
    }

    template <typename TYPE>
    static void UnitTestMatch() {  // static
        StrLen_t nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("*.c"), false);
        UNITTEST_TRUE(nLenMatch == 5);
        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("*.c.d"), false);
        UNITTEST_TRUE(nLenMatch == 0);
        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("*.d"), false);
        UNITTEST_TRUE(nLenMatch == 0);

        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("*.b.*"), false);
        UNITTEST_TRUE(nLenMatch == 5);

        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("a*b"), false, 5);
        UNITTEST_TRUE(nLenMatch == 3);
        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("A*B"), true, 5);
        UNITTEST_TRUE(nLenMatch == 3);
        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("A*B"), false, 5);
        UNITTEST_TRUE(nLenMatch == 0);
        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("a.b.c.d"), false, 5);
        UNITTEST_TRUE(nLenMatch == -5);
        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("*.b.c.d"), false, 5);
        UNITTEST_TRUE(nLenMatch == -5);
        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("*.d"), false, 5);
        UNITTEST_TRUE(nLenMatch == 0);
        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("*c.d"), false, 5);
        UNITTEST_TRUE(nLenMatch == -5);

        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("d.*"), false);
        UNITTEST_TRUE(nLenMatch == 0);
        nLenMatch = StrT::MatchRegEx<TYPE>(k_t3, CSTRCONST("*.d"), false);
        UNITTEST_TRUE(nLenMatch == 0);
    }

    template <typename TYPE>
    static void UnitTestTable() {  // static

        cSpanUnk tableEmpty(cBlob::k_Empty, _SIZEOF_PTR);

        UNITTEST_TRUE(StrX<TYPE>::IsTableSorted(tableEmpty));

        ITERATE_t iCountRet = StrT::TableFind<TYPE>(k_t3, tableEmpty);
        UNITTEST_TRUE(iCountRet == -1);

        iCountRet = StrT::TableFindHead<TYPE>(k_t3, tableEmpty);
        UNITTEST_TRUE(iCountRet == -1);

        iCountRet = StrT::TableFindSorted<TYPE>(k_t3, tableEmpty);
        UNITTEST_TRUE(iCountRet == -1);

        iCountRet = StrT::TableFindHeadSorted<TYPE>(k_t3, tableEmpty);
        UNITTEST_TRUE(iCountRet == -1);

        static const TYPE* kA[1] = {k_t3};
        cSpan<const TYPE*> kSpan = TOSPAN(kA);
        cSpanUnk table2(kSpan.get_SpanUnk());

        iCountRet = StrT::TableFindSorted<TYPE>(k_t3, table2);
        // UNITTEST_TRUE(iCountRet == -1);
        iCountRet = StrT::TableFindHeadSorted<TYPE>(k_t3, table2);
        // UNITTEST_TRUE(iCountRet == -1);
    }

    template <typename TYPE>
    static void UnitTestModify() {  // static

        TYPE szTmp[StrT::k_LEN_Default];
        StrLen_t nLenRet = StrT::CopyLen<TYPE>(szTmp, k_t1, 16);
        UNITTEST_TRUE(nLenRet == 7);

        nLenRet = StrT::CopyPtr<TYPE>(TOSPAN(szTmp), CSTRCONST("123 "));
        UNITTEST_TRUE(nLenRet == 4);

        const TYPE* pRetChar = StrT::TrimWhitespace(szTmp);
        UNITTEST_TRUE(pRetChar == szTmp);
        UNITTEST_TRUE(StrT::Len(szTmp) == 3);
        pRetChar = StrT::StripBlock(szTmp);
        UNITTEST_TRUE(pRetChar != nullptr && *pRetChar == '1');  // ""

        nLenRet = StrT::CopyPtr<TYPE>(TOSPAN(szTmp), CSTRCONST("this are a string"));  // sic
        UNITTEST_TRUE(nLenRet);

        StrT::ReplaceX<TYPE>(TOSPAN(szTmp), 5, 3, ToSpanStr<TYPE>(CSTRCONST("is")));
        UNITTEST_TRUE(!StrT::Cmp<TYPE>(szTmp, ToSpanStr<TYPE>(CSTRCONST("this is a string"))));
    }

    template <typename TYPE>
    static void UnitTestParse() {  // static
        TYPE szTmp[StrT::k_LEN_Default];
        TYPE* ppCmds[128];
        ITERATE_t iCountRet = StrT::ParseArrayTmp<TYPE>(TOSPAN(szTmp), CSTRCONST("0"), TOSPAN(ppCmds), nullptr, STRP_DEF);
        UNITTEST_TRUE(iCountRet == 1);
        UNITTEST_TRUE(ppCmds[0] == szTmp);

        const TYPE* pszCmd = CSTRCONST("0 ,1 ,2.234,3.0 ");
        iCountRet = StrT::ParseArrayTmp<TYPE>(TOSPAN(szTmp), pszCmd, cSpanX<TYPE*>(ppCmds, 3), nullptr, STRP_DEF);
        UNITTEST_TRUE(iCountRet == 3);
        UNITTEST_TRUE(ppCmds[2] == szTmp + 6);
    }

    template <typename TYPE>
    static StrLen_t TestFill(cSpanX<TYPE>& tmp) {
        for (int i = 0; i < tmp.GetSize(); i++) tmp.get_PtrWork()[i] = (TYPE)(i + 1);
        tmp.get_PtrWork()[tmp.GetSize() - 1] = '\0';
        return StrT::Len<TYPE>(tmp);
    }

    template <typename TYPE>
    static void UnitTestESC(const cSpan<TYPE>& src, StrLen_t escaped) {  // static
        TYPE szTmpE1[1024];
        StrLen_t iLenE = StrT::EscSeqAdd<TYPE>(TOSPAN(szTmpE1), src);
        UNITTEST_TRUE(iLenE == src.GetSize() + escaped);

        TYPE szTmpD1[STRMAX(szTmpE1)];
        StrLen_t iLenD = StrT::EscSeqDecode<TYPE>(TOSPAN(szTmpD1), szTmpE1);
        UNITTEST_TRUE(iLenD == src.GetSize() + escaped);
        UNITTEST_TRUE(!StrT::Cmp<TYPE>(szTmpD1, src));

        TYPE szTmpE2[STRMAX(szTmpE1)];
        iLenE = StrT::EscSeqAddQ<TYPE>(TOSPAN(szTmpE2), src);  // Quoted
        UNITTEST_TRUE(iLenE == src.GetSize() + escaped + 2);

        TYPE szTmpD2[STRMAX(szTmpE1)];
        iLenD = StrT::EscSeqDecodeQ<TYPE>(TOSPAN(szTmpD2), szTmpE2, cStrConst::k_LEN_MAX);
        UNITTEST_TRUE(iLenD == src.GetSize() + escaped + 2);
    }

    template <typename TYPE>
    static void UnitTestESC() {  // static
        // decode
        static cStrConst k_tEsc1 = CSTRCONST("\\n\\r\\44\\x23");
        static cStrConst k_tEsc2 = CSTRCONST("\n\r\44\x23");
        TYPE szTmpD1[128];
        StrLen_t iLenD = StrT::EscSeqDecode<TYPE>(TOSPAN(szTmpD1), k_tEsc1);
        UNITTEST_TRUE(iLenD == StrT::Len<TYPE>(k_tEsc1));
        UNITTEST_TRUE(!StrT::Cmp<TYPE>(szTmpD1, k_tEsc2));

        // Test
        TYPE szTmpAscii[128];  // @todo > 127?
        UNITTEST_TRUE(STRMAX(szTmpAscii) == 127);
        UNITTEST_TRUE(TestFill(TOSPAN(szTmpAscii)) == 127);
        UnitTestESC(StrT::ToSpanStr<TYPE>(szTmpAscii), 103);

        //
        static cStrConst k_IntQ = CSTRCONST("int\"quote");
        UnitTestESC(StrT::ToSpanStr<TYPE>(k_IntQ), 1);

        static cStrConst k_None = CSTRCONST("none");
        UnitTestESC(StrT::ToSpanStr<TYPE>(k_None), 0);

        // Test
        static cStrConst k_tEsc = CSTRCONST("sd\nf\ts\"3\"d\tf\\2\n");
        const StrLen_t iLenStr = StrT::Len<TYPE>(k_tEsc);
        UNITTEST_TRUE(iLenStr == 15);
        UnitTestESC(StrT::ToSpanStr<TYPE>(k_tEsc), 7);
    }

    template <typename TYPE>
    static void UnitTestStrT() {  // static

        UnitTestBasic<TYPE>();
        UnitTestFind<TYPE>();
        UnitTestInt<TYPE>();
        UnitTestDouble<TYPE>();
        UnitTestMatch<TYPE>();
        UnitTestTable<TYPE>();
        UnitTestModify<TYPE>();
        UnitTestParse<TYPE>();
        UnitTestESC<TYPE>();

        UNITTEST_TRUE(StrT::Cmp<TYPE>(StrX<TYPE>::GetBoolStr(true), CSTRCONST("1")) == 0);
        UNITTEST_TRUE(StrT::Cmp<TYPE>(StrX<TYPE>::GetBoolStr(false), CSTRCONST("0")) == 0);

        // tests for StrT::sprintfN<TYPE> are in StrFormat
    }

    void TestHashCode() {
        const auto h1a = StrT::GetHashCode32(TOSPAN_LIT("arrastra"));
        const auto h1b = StrT::GetHashCode32(TOSPAN_LIT("asclepius"));
        UNITTEST_TRUE(h1a == h1b);  // known collision.

        // 'asclepius' = 'arrastra'
        // 'bencher' = 'barkhan'
        // 'confirmity' = 'adroitness'
        // 'lumbers' = 'befortune'
        // 'megalocornea' = 'arsenic'
        // 'nonpacifist' = 'nonfeverishness'
        // 'odontoceti' = 'entozoologically'
        // 'phoresis' = 'madeiran'
        // 'preevidence' = 'gravelled'
        // 'questionlessness' = 'nothingness'
        // 'sandiferous' = 'phrasiness'
        // 'shute' = 'biland'
        // 'sycamores' = 'phoradendron'
        // 'tenuirostres' = 'barmfel'
        // 'topazes' = 'estragole'
    }

    UNITTEST_METHOD(StrT) {
        TestHashCode();
        UnitTestStrT<char>();
        UnitTestStrT<wchar_t>();
    }
};
UNITTEST2_REGISTER(StrT, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
