//
//! @file cAtom.Tests.cpp
//
#include "pch.h"
#include "cAtom.h"
#include "cAtomManager.h"

namespace Gray
{
	UNITTEST2_CLASS(cAtom)
	{
		UNITTEST2_METHOD(cAtom)
		{
			cAtomManager& rAM = cAtomManager::I();

			//! empty
			cAtomRef a0 = CATOM_STR("");	// empty hash.
			cAtomRef a02 = cAtomRef::FindAtomStr(CATOM_STR(""));
			UNITTEST2_TRUE(a0.get_HashCode() == 0);
			UNITTEST2_TRUE( rAM.m_aEmpty == a0 );

			//! test words
			UNITTEST2_TRUE(cAtomRef::FindAtomStr(CATOM_STR("test1")).IsEmpty());		// 
			cAtomRef a2(CATOM_STR("test1"));
			UNITTEST2_TRUE(!cAtomRef::FindAtomStr(CATOM_STR("test1")).IsEmpty());		// 
			cAtomRef a1 = CATOM_STR("test2");
			a2 = CATOM_STR("test3");
			a2 = a1;
			a1 = CATOM_STR("test3");

			UNITTEST2_TRUE(cAtomRef::FindAtomStr(CATOM_STR("test1")).IsEmpty());		// should be gone!
			UNITTEST2_TRUE(!cAtomRef::FindAtomStr(CATOM_STR("test3")).IsEmpty());	// should find it.

			//! k_asTextLines
			{
				cAtomRef aa[cUnitTestCur::k_TEXTLINES_QTY];
				for (ITERATE_t i = 0; i < cUnitTestCur::k_TEXTLINES_QTY; i++)
				{
					aa[i] = k_asTextLines[i];
				}

				rAM.DebugDumpFile(cStreamNull());
			}

			UNITTEST2_TRUE(rAM.m_aName.isArraySorted());

			// Test hash codes.
			ATOMCODE_t nHashCode = a1.get_HashCode();
			a2 = cAtomRef::FindAtomHashCode(nHashCode);
			UNITTEST2_TRUE(!a2.IsEmpty());

			// Test bad hash code.
			a2 = cAtomRef::FindAtomHashCode(0x123123);
			UNITTEST2_TRUE(a2.IsEmpty());

			ATOMCHAR_t szTmp[_MAX_PATH];
			static const ATOMCHAR_t* k_t2 = CATOM_STR("sdfsdF23 5");	// lower case = higher number ASCII.
			StrLen_t nLen = cAtomRef::GetSymbolicStr(szTmp, k_t2, false);
			UNITTEST2_TRUE(nLen == 8);

			HRESULT hRes = cAtomRef::CheckSymbolicStr(szTmp);
			UNITTEST2_TRUE(SUCCEEDED(hRes));

			hRes = cAtomRef::CheckSymbolicStr(CATOM_STR("sdfsdf"));
			UNITTEST2_TRUE(hRes == 6);

			hRes = cAtomRef::CheckSymbolicStr(CATOM_STR("sdfsdf s"));
			UNITTEST2_TRUE(hRes == E_INVALIDARG);

		}
	};
	UNITTEST2_REGISTER(cAtom, UNITTEST_LEVEL_Core);

}
