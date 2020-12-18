//
//! @file StrA.Tests.cpp
//
#include "pch.h"
#include "StrA.h"
#include "cIniBase.h"

namespace Gray
{
	UNITTEST2_CLASS(StrA)
	{
		UNITTEST2_METHOD(StrA)
		{
			UNITTEST2_TRUE(StrA::IsBoolTrue("true"));
			UNITTEST2_TRUE(!StrA::IsBoolTrue("sdf"));
			UNITTEST2_TRUE(StrA::IsBoolTrue("1"));
			UNITTEST2_TRUE(!StrA::IsBoolTrue("0"));
			UNITTEST2_TRUE(StrA::IsBoolTrue("True"));
			UNITTEST2_TRUE(!StrA::IsBoolFalse("true"));
			UNITTEST2_TRUE(!StrA::IsBoolTrue("false"));
			UNITTEST2_TRUE(StrA::IsBoolFalse("false"));

			UNITTEST2_TRUE(StrA::IsPlural("hair"));
			UNITTEST2_TRUE(!StrA::IsPlural("foot"));
			UNITTEST2_TRUE(StrA::IsPlural("feet"));

			// StrA::MakeNamedBitmask
			static const char* k_aszNames[] =
			{
				"0", "1", "2", "3",
				"4", "5", "6", "7",
				"8", "9", "10", "11",
			};
			char szTmp1[_MAX_PATH];
			StrLen_t iRet2 = StrA::MakeNamedBitmask(szTmp1, STRMAX(szTmp1), 0x123, k_aszNames, _countof(k_aszNames));
			UNITTEST2_TRUE(iRet2 >= 7);
			// "0,1,5,8"

			// StrA::BlockReplacement
			class cUnitTestBlock : public IIniBaseGetter
			{
			public:
				virtual HRESULT PropGet(const char* pszPropTag, OUT cStringI& rsVal) const
				{
					if (!Gray::StrT::CmpI(pszPropTag, "blocks"))
					{
						rsVal = "TESTBLOCK";
						return S_OK;
					}
					if (!Gray::StrT::CmpI(pszPropTag, "blo1"))
					{
						rsVal = "blo";
						return S_OK;
					}
					if (!Gray::StrT::CmpI(pszPropTag, "cks2"))
					{
						rsVal = "cks";
						return S_OK;
					}
					return HRESULT_WIN32_C(ERROR_UNKNOWN_PROPERTY);
				}
			} testBlock;

			char szTmp2[StrT::k_LEN_MAX];
			StrLen_t iRet3 = StrA::BlockReplacement(szTmp2, STRMAX(szTmp2),
				"Test with recursive <?<?blo1?><?cks2?>?>. and junk <?IntentionalUnknownProperty?>.",
				&testBlock, false);
			UNITTEST2_TRUE(iRet3 == 71);

			StrLen_t iRet1 = StrA::BlockReplacement(szTmp1, STRMAX(szTmp1),
				"Test input string with <?blocks?>. And another <?blocks?>.",
				&testBlock, false);
			UNITTEST2_TRUE(iRet1 == 56);
		}
	};

	UNITTEST2_REGISTER(StrA, UNITTEST_LEVEL_Core);
}
