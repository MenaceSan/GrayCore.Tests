//
//! @file cAtom.Tests.cpp
//
#include "pch.h"
#include "cAtom.h"
#include "cAtomManager.h"

namespace Gray
{
	static const ATOMCHAR_t k_empty[] = CATOM_STR("");
	static const ATOMCHAR_t k_Root[] = CATOM_N(Root);
	static const cAtomRef a_Root(k_Root);	// define atom globally
	static const ATOMCHAR_t k_test1[] = CATOM_N(test1);
	static const ATOMCHAR_t k_test2[] = CATOM_N(test2);
	static const ATOMCHAR_t k_test3[] = CATOM_N(test3);

	UNITTEST_CLASS(cAtom)
	{
		void TestHash()
		{
			// Test hash codes.
			cAtomRef a1(k_test1);
			ATOMCODE_t nHashCode = a1.get_HashCode();
			cAtomRef a2 = cAtomRef::FindAtomHashCode(nHashCode);
			UNITTEST_TRUE(!a2.IsEmpty());

			// Test bad hash code.
			a2 = cAtomRef::FindAtomHashCode(0x123123);
			UNITTEST_TRUE(a2.IsEmpty());
		}

		void TestNames()
		{
			ATOMCHAR_t szTmp[_MAX_PATH];
			static const ATOMCHAR_t* k_t2 = CATOM_STR("sdfsdF23 5");	// lower case = higher number ASCII.
			HRESULT hRes = cAtomRef::CheckSymbolicStr(k_t2);
			UNITTEST_TRUE(hRes == E_INVALIDARG);
			StrLen_t nLen = cAtomRef::GetSymbolicStr(szTmp, k_t2, false);
			UNITTEST_TRUE(nLen == 8);

			hRes = cAtomRef::CheckSymbolicStr(szTmp);
			UNITTEST_TRUE(SUCCEEDED(hRes));

			hRes = cAtomRef::CheckSymbolicStr(CATOM_N(sdfsdf));
			UNITTEST_TRUE(hRes == 6);
		}

		void TestEmpty()
		{
			//! empty
			cAtomManager& rAM = cAtomManager::I();
			cAtomRef a0 = k_empty;	// empty hash.
			cAtomRef a02 = cAtomRef::FindAtomStr(k_empty);
			UNITTEST_TRUE(a0.get_HashCode() == 0);
			UNITTEST_TRUE(rAM.m_aEmpty == a0);
		}

		void TestRoot()
		{
			// a_Root
			cAtomRef aRoot = cAtomRef::FindAtomStr(k_Root);
			UNITTEST_TRUE(!aRoot.IsEmpty());		// k_Root
			UNITTEST_TRUE(aRoot == a_Root);
		}

		void TestFree()
		{
			// test1
			//! test words
			UNITTEST_TRUE(cAtomRef::FindAtomStr(k_test1).IsEmpty());		// 
			cAtomRef a2(k_test1);
			UNITTEST_TRUE(!cAtomRef::FindAtomStr(k_test1).IsEmpty());		// 

			cAtomRef a1 = k_test2;
			a2 = k_test3;
			a2 = a1;
			a1 = k_test3;

			UNITTEST_TRUE(cAtomRef::FindAtomStr(k_test1).IsEmpty());		// should be gone! no more refs to it. kRefsBase
			UNITTEST_TRUE(!cAtomRef::FindAtomStr(k_test3).IsEmpty());	// should find k_test3.
		}

		void TestBatch()
		{
			// create a bunch
			//! k_asTextLines
			cAtomManager& rAM = cAtomManager::I();

			cAtomRef aa[cUnitTestCur::k_TEXTLINES_QTY];
			for (ITERATE_t i = 0; i < cUnitTestCur::k_TEXTLINES_QTY; i++)
			{
				aa[i] = k_asTextLines[i];
			}

			rAM.DebugDumpFile(cStreamNull());
		}

		UNITTEST_METHOD(cAtom)
		{
			cAtomManager& rAM = cAtomManager::I();

			TestNames();
			TestHash();
			TestEmpty();
			TestRoot();
			TestFree();
			TestBatch();

			UNITTEST_TRUE(rAM.DebugTest());
		}
	};
	UNITTEST2_REGISTER(cAtom, UNITTEST_LEVEL_Core);

}
