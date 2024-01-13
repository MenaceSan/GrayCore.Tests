//
//! @file StrT.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/StrT.h>
#include <GrayCore/include/StrU.h>

namespace Gray {
static const cStrConst k_t1 = CSTRCONST("sdfsdf1");
static const cStrConst k_t2 = CSTRCONST("sdfsdF23 5");  // lower case = higher number ASCII.
static const cStrConst k_t3 = CSTRCONST("a.b.c");

struct UNITTEST_N(StrT) : public cUnitTest {
    template <typename TYPE>
    static void UnitTestBasic()  // static
    {
        // Basic tests first.
        STATIC_ASSERT('\n' == 0x0a, Check_NL);  // StrChar::k_NL
        STATIC_ASSERT('\r' == 0x0d, Check_CR);  // StrChar::k_CR

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

        StrLen_t nLen = StrT::Len<TYPE>(k_t1);
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

        bCmp = StrT::EndsWithI<TYPE>(k_t2, k_t1);
        UNITTEST_TRUE(!bCmp);

        HASHCODE32_t nHashCode = StrT::GetHashCode32<TYPE>(k_t1, k_StrLen_UNK, 0);
        UNITTEST_TRUE(nHashCode == 0x1488c5b4);
    }

    template <typename TYPE>
    static void UnitTestFind()  // static
    {
        static const cStrConst k_tSent = CSTRCONST("This is a sentence. And another. // comment");

        const TYPE* pRetChar = StrT::FindChar<TYPE>(k_t1, (TYPE)'f');
        UNITTEST_TRUE(pRetChar != nullptr && *pRetChar == 'f');

        pRetChar = StrT::GetNonWhitespace<TYPE>(k_tSent);
        UNITTEST_TRUE(pRetChar != nullptr);

        StrLen_t nLen = StrT::FindCharN<TYPE>(k_t1, (TYPE)'f');
        UNITTEST_TRUE(nLen == 2);

        pRetChar = StrT::FindCharRev<TYPE>(k_t1, (TYPE)'f');
        UNITTEST_TRUE(pRetChar != nullptr && *pRetChar == 'f');  // f1

        nLen = StrT::FindWord<TYPE>(k_tSent, CSTRCONST("sentence"));
        UNITTEST_TRUE(nLen == 18);

        pRetChar = StrT::FindTokens<TYPE>(k_tSent, k_t2);
        UNITTEST_TRUE(pRetChar != nullptr);

        const TYPE* pszTest = CSTRCONST("abcdefabcdefg");
        const TYPE* pszRet = StrT::FindStr<TYPE>(pszTest, CSTRCONST("abcdefg"));
        UNITTEST_TRUE(pszRet == pszTest + 6);

        pszTest = CSTRCONST("abcabcabcabc");
        pszRet = StrT::FindStr<TYPE>(pszTest, CSTRCONST("abca"));
        UNITTEST_TRUE(pszRet == pszTest + 0);

        pszRet = StrT::FindStrI<TYPE>(pszTest, CSTRCONST("AbCa"));
        UNITTEST_TRUE(pszRet == pszTest + 0);
    }

    template <typename TYPE>
    static void UnitTestInt()  // static
    {
        UINT64 ulVal = StrT::toUL<TYPE>(CSTRCONST("0xFFFFFFFF"), nullptr, 8);
        UNITTEST_TRUE(ulVal == 0xFFFFFFFF);
        ulVal = StrT::toUL<TYPE>(CSTRCONST("0xFFFFFFFF"), nullptr);
        UNITTEST_TRUE(ulVal == 0xFFFFFFFF);

        ulVal = StrT::toUL<TYPE>(CSTRCONST("FFFFFFFF"), nullptr);
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
        StrLen_t nLenRet = StrT::ILtoA<TYPE>(123123, szTmp, STRMAX(szTmp), 10);
        UNITTEST_TRUE(!StrT::Cmp<TYPE>(szTmp, CSTRCONST("123123")));

        nLenRet = StrT::ULtoAK<TYPE>(123123, szTmp, STRMAX(szTmp), 1024, true);
        UNITTEST_TRUE(nLenRet == 8);
    }

    template <typename TYPE>
    static void UnitTestDouble()  // static
    {
        // Test float,double.  Test MUST be reversible.
        TYPE szTmp[StrNum::k_LEN_MAX_DIGITS + 32];

        const cStrConst kValS = CSTRCONST("123.123");
        const double kValN = 123.123;

        double dVal = StrT::toDouble<TYPE>(kValS, nullptr);
        UNITTEST_TRUE(dVal == kValN);  // NOT 123.12299999

        StrLen_t nLenRet = StrT::DtoA<TYPE>(dVal, szTmp, STRMAX(szTmp));
        UNITTEST_TRUE(!StrT::Cmp<TYPE>(szTmp, kValS));
    }

    template <typename TYPE>
    static void UnitTestMatch()  // static
    {
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
    static void UnitTestTable()  // static
    {
        // No TYPE arg.
        const TYPE* pszRet = StrX<TYPE>::GetTableElem(0, nullptr, 0, 0);
        UNITTEST_TRUE(pszRet != nullptr && *pszRet == '?');

        // StrX<TYPE>::GetTableElem();

        ITERATE_t iCountRet = StrX<TYPE>::GetTableCount(nullptr, 0);
        UNITTEST_TRUE(iCountRet == 0);

        iCountRet = StrX<TYPE>::GetTableCountSorted(nullptr, 0);
        UNITTEST_TRUE(iCountRet == 0);

        iCountRet = StrT::TableFind<TYPE>(k_t3, nullptr, 0);
        UNITTEST_TRUE(iCountRet == -1);

        iCountRet = StrT::TableFindHead<TYPE>(k_t3, nullptr, 0);
        UNITTEST_TRUE(iCountRet == -1);

        iCountRet = StrT::TableFindSorted<TYPE>(k_t3, nullptr, 0);
        UNITTEST_TRUE(iCountRet == -1);

        iCountRet = StrT::TableFindHeadSorted<TYPE>(k_t3, nullptr, 0);
        UNITTEST_TRUE(iCountRet == -1);
    }

    template <typename TYPE>
    static void UnitTestModify()  // static
    {
        TYPE szTmp[StrT::k_LEN_Default];
        StrLen_t nLenRet = StrT::CopyLen<TYPE>(szTmp, k_t1, 16);
        UNITTEST_TRUE(nLenRet == 7);

        nLenRet = StrT::CopyLen<TYPE>(szTmp, CSTRCONST("123 "), STRMAX(szTmp));
        UNITTEST_TRUE(nLenRet == 4);

        const TYPE* pRetChar = StrT::TrimWhitespace(szTmp);
        UNITTEST_TRUE(pRetChar == szTmp);
        UNITTEST_TRUE(StrT::Len(szTmp) == 3);
        pRetChar = StrT::StripBlock(szTmp);
        UNITTEST_TRUE(pRetChar != nullptr && *pRetChar == '1');  // ""

        nLenRet = StrT::CopyLen<TYPE>(szTmp, CSTRCONST("this are a string"), STRMAX(szTmp));  // sic
        UNITTEST_TRUE(nLenRet);

        StrT::ReplaceX<TYPE>(szTmp, STRMAX(szTmp), 5, 3, CSTRCONST("is"));
        UNITTEST_TRUE(!StrT::Cmp<TYPE>(szTmp, CSTRCONST("this is a string")));
    }

    template <typename TYPE>
    static void UnitTestParse()  // static
    {
        TYPE szTmp[StrT::k_LEN_Default];
        TYPE* ppCmds[128];
        ITERATE_t iCountRet = StrT::ParseArrayTmp<TYPE>(szTmp, STRMAX(szTmp), CSTRCONST("0"), ppCmds, _countof(ppCmds), nullptr, STRP_DEF);
        UNITTEST_TRUE(iCountRet == 1);
        UNITTEST_TRUE(ppCmds[0] == szTmp);

        const TYPE* pszCmd = CSTRCONST("0 ,1 ,2.234,3.0 ");
        iCountRet = StrT::ParseArrayTmp<TYPE>(szTmp, STRMAX(szTmp), pszCmd, ppCmds, 3, nullptr, STRP_DEF);
        UNITTEST_TRUE(iCountRet == 3);
        UNITTEST_TRUE(ppCmds[2] == szTmp + 6);
    }

    template <typename TYPE>
    static void UnitTestESC()  // static
    {
        static cStrConst k_tEsc = CSTRCONST("sd\nf\tsd\tf2\n");
        const StrLen_t iLenStr = StrT::Len<TYPE>(k_tEsc);
        UNITTEST_TRUE(iLenStr == 11);

        // @todo > 127?
        TYPE szTmpEsc[127];
        UNITTEST_TRUE(STRMAX(szTmpEsc) == 126);
        for (int i = 0; i < (int)STRMAX(szTmpEsc); i++) szTmpEsc[i] = (TYPE)(i + 1);
        szTmpEsc[STRMAX(szTmpEsc)] = '\0';
        const StrLen_t iLenEsc = StrT::Len<TYPE>(szTmpEsc);
        UNITTEST_TRUE(iLenEsc == 126);

        TYPE szTmpE1[128];
        StrLen_t iLenE = StrT::EscSeqAdd<TYPE>(szTmpE1, k_tEsc, STRMAX(szTmpE1));
        UNITTEST_TRUE(iLenE == 15);

        TYPE szTmpE2[1024];
        iLenE = StrT::EscSeqAdd<TYPE>(szTmpE2, szTmpEsc, STRMAX(szTmpE2));
        UNITTEST_TRUE(iLenE == 225);  // 225 for 127, 725 for ?

        TYPE szTmpD1[STRMAX(szTmpE1)];
        StrLen_t iLenD = StrT::EscSeqRemove<TYPE>(szTmpD1, szTmpE1, STRMAX(szTmpD1));
        UNITTEST_TRUE(iLenD == iLenStr);
        UNITTEST_TRUE(!StrT::Cmp<TYPE>(szTmpD1, k_tEsc));

        static const cStrConst k_Q = CSTRCONST("\"abcdefgh\"");
        const TYPE* pszQ = k_Q;
        iLenD = StrT::EscSeqRemoveQ<TYPE>(szTmpD1, pszQ, STRMAX(szTmpD1), StrT::k_LEN_MAX);
        UNITTEST_TRUE(iLenD == 10);

        iLenE = StrT::EscSeqAddQ<TYPE>(szTmpE1, k_tEsc, STRMAX(szTmpE1));  // Quoted
        UNITTEST_TRUE(iLenE == 17);

        TYPE szTmpD2[STRMAX(szTmpE2)];
        iLenD = StrT::EscSeqRemove<TYPE>(szTmpD2, szTmpE2, STRMAX(szTmpD2));
        //	UNITTEST_TRUE(iLenD == _countof(szTmpEsc));
        for (int i = 0; i < (int)STRMAX(szTmpEsc); i++) {
            UNITTEST_TRUE(szTmpEsc[i] == (TYPE)(i + 1));
            UNITTEST_TRUE(szTmpD2[i] == (TYPE)(i + 1));
        }
        UNITTEST_TRUE(szTmpEsc[STRMAX(szTmpEsc)] == '\0');
        UNITTEST_TRUE(szTmpD2[STRMAX(szTmpEsc)] == '\0');
    }

    template <typename TYPE>
    static void UnitTestStrT()  // static
    {
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
        const auto h1a = StrT::GetHashCode32("arrastra");
        const auto h1b = StrT::GetHashCode32("asclepius");
        UNITTEST_TRUE(h1a == h1b);

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
