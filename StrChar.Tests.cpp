// 
//! @file StrChar.Tests.cpp
//
#include "pch.h"
#include "StrChar.h"

namespace Gray
{
	UNITTEST_CLASS(StrChar)
	{
		UNITTEST_METHOD(StrChar)
		{
			UNITTEST_TRUE(StrChar::IsSpace(' ') == true);
			UNITTEST_TRUE(StrChar::IsSpace('a') == false);
			UNITTEST_TRUE(StrChar::IsSpace('\0') == false);

			for (WORD uRadix = StrChar::k_uRadixMin; uRadix <= StrChar::k_uRadixMax; uRadix++)
			{
				for (UINT uVal = 0; uVal < uRadix; uVal++)
				{
					char ch = StrChar::U2Radix(uVal, uRadix);
					UINT uVal2 = StrChar::Radix2U(ch, uRadix);
					UNITTEST_TRUE(uVal2 == uVal);
				}

				UNITTEST_TRUE(StrChar::U2Radix(uRadix + 1, uRadix) == '?');
				UNITTEST_TRUE(StrChar::Radix2U('?', uRadix) == -1);
			}
		}
	};
	UNITTEST2_REGISTER(StrChar, UNITTEST_LEVEL_Core);
}
