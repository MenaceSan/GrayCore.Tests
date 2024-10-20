//! @file cAtom.Tests.cpp

#include "pch.h"
#include <GrayCore/include/StrA.h>
#include <GrayCore/include/cAtom.h>
#include <GrayCore/include/cAtomManager.h>

namespace Gray {
static const ATOMCHAR_t k_empty[] = CATOM_STR("");  // k_aEmpty
static const ATOMCHAR_t k_Root[] = CATOM_N(Root);
static const cAtomRef a_Root(k_Root);  // define atom globally
static const ATOMCHAR_t k_test1[] = CATOM_N(test1);
static const ATOMCHAR_t k_test2[] = CATOM_N(test2);
static const ATOMCHAR_t k_test3[] = CATOM_N(test3);

struct UNITTEST_N(cAtom) : public cUnitTest {
    void TestHash(const ATOMCHAR_t* a) {  // Test hash codes.

        const cSpan<ATOMCHAR_t> sp(StrT::ToSpanStr(a));
        const ATOMCODE_t nHashCode0 = StrT::GetHashCode32(sp);
        UNITTEST_TRUE(nHashCode0 != 0);
        cAtomRef a0 = cAtomRef::FindAtomHashCode(nHashCode0);  // already used ?

        const cAtomRef a1(a);
        const ATOMCODE_t nHashCode1 = a1.get_HashCode();
        UNITTEST_TRUE(nHashCode0 == nHashCode1);

        cStringA sa2 = a;
        UNITTEST_TRUE(sa2.Compare(a) == 0);
        const ATOMCHAR_t ch = sa2[1];
        sa2.SetAt(1, StrChar::IsUpper(ch) ? StrChar::ToLower(ch) : StrChar::ToUpper(ch));
        UNITTEST_TRUE(sa2.Compare(a) != 0);
        UNITTEST_TRUE(sa2.CompareNoCase(a) == 0);
        cAtomRef a2(sa2);  // Case change should not change hash!
        UNITTEST_TRUE(a2.get_HashCode() == nHashCode1);

        cAtomRef a1sp(sp);
        UNITTEST_TRUE(a1sp.get_HashCode() == nHashCode1);

        cAtomRef a1n = cAtomRef::FindAtomHashCode(nHashCode1);
        UNITTEST_TRUE(!a1n.IsEmpty());
        UNITTEST_TRUE(a1n.get_HashCode() == nHashCode1);

        cAtomRef a1s = cAtomRef::FindAtomStr(a);
        UNITTEST_TRUE(!a1s.IsEmpty());
        UNITTEST_TRUE(a1s.get_HashCode() == nHashCode1);
    }

    void TestHash() {  // Test hash codes.
        TestHash(k_test1);
        TestHash(k_test2);

        // Test bad hash code.
        cAtomRef a2 = cAtomRef::FindAtomHashCode(0x123123);
        UNITTEST_TRUE(a2.IsEmpty());
    }

    void TestNames() {
        ATOMCHAR_t szTmp[cFilePath::k_MaxLen];
        static const ATOMCHAR_t* k_t2 = CATOM_STR("sdfsdF23 5");  // lower case = higher number ASCII.
        HRESULT hRes = StrA::CheckSymName(k_t2);
        UNITTEST_TRUE(hRes == E_INVALIDARG);
        StrLen_t nLen = cAtomRef::MakeSymName(TOSPAN(szTmp), k_t2, false);
        UNITTEST_TRUE(nLen == 8);

        hRes = StrA::CheckSymName(szTmp);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        hRes = StrA::CheckSymName(CATOM_N(sdfsdf));
        UNITTEST_TRUE(hRes == 6);
    }

    void TestEmpty() {
        //! empty
        cAtomManager& rAM = cAtomManager::I();
        cAtomRef a0;
        cAtomRef a1 = k_empty;
        cAtomRef a2 = cAtomRef::FindAtomStr(k_empty);
        cAtomRef a3("");
        cAtomRef a4(cStringA(""));

        UNITTEST_TRUE(a0.get_HashCode() == k_HASHCODE_CLEAR);
        UNITTEST_TRUE(a1.get_HashCode() == k_HASHCODE_CLEAR);
        UNITTEST_TRUE(a2.get_HashCode() == k_HASHCODE_CLEAR);
        UNITTEST_TRUE(a3.get_HashCode() == k_HASHCODE_CLEAR);
        UNITTEST_TRUE(a4.get_HashCode() == k_HASHCODE_CLEAR);

        UNITTEST_TRUE(a0.get_StrA().IsEmpty());
        UNITTEST_TRUE(a0.get_StrA().GetLength() == 0);
    }

    void TestRoot() {
        UNITTEST_TRUE(a_Root.GetLength() == 4);
        const ATOMCODE_t nHashCode1 = a_Root.get_HashCode();
        UNITTEST_TRUE(nHashCode1 == 0xf0e12463);

        cAtomRef aRoot = cAtomRef::FindAtomStr(k_Root);
        UNITTEST_TRUE(!aRoot.IsEmpty());  // k_Root
        UNITTEST_TRUE(aRoot == a_Root);
        // KNown Hash
        const cAtomRef aRoot3("Root");
        UNITTEST_TRUE(aRoot3 == a_Root);
        const cAtomRef aRoot2("root");
        UNITTEST_TRUE(aRoot2 == a_Root);
    }

    void TestFree() {
        // test1
        //! test words
        UNITTEST_TRUE(cAtomRef::FindAtomStr(k_test1).IsEmpty());  //
        cAtomRef a2(k_test1);
        UNITTEST_TRUE(!cAtomRef::FindAtomStr(k_test1).IsEmpty());  //

        cAtomRef a1 = k_test2;
        a2 = k_test3;
        a2 = a1;
        a1 = k_test3;

        UNITTEST_TRUE(cAtomRef::FindAtomStr(k_test1).IsEmpty());   // should be gone! no more refs to it. kRefsBase
        UNITTEST_TRUE(!cAtomRef::FindAtomStr(k_test3).IsEmpty());  // should find k_test3.
    }

    void TestBatch() {
        // create a bunch
        //! k_asTextLines
        cAtomManager& rAM = cAtomManager::I();

        cAtomRef aa[_countof(k_asTextLines)];
        for (ITERATE_t i = 0; i < _countof(k_asTextLines); i++) {
            aa[i] = k_asTextLines[i];
        }

        rAM.DebugDumpFile(cStreamNull::I());
    }

    UNITTEST_METHOD(cAtom) {
        cAtomManager& rAM = cAtomManager::I();

        TestNames();
        TestHash();
        TestEmpty();
        TestRoot();
        TestFree();
        TestBatch();

        UNITTEST_TRUE(rAM.DebugTestPoint());
    }
};
UNITTEST2_REGISTER(cAtom, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
