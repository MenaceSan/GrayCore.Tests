// 
//! @file StrChar.Tests.cpp
//
#include "pch.h"
#include "StrChar.h"

namespace Gray
{
	UNITTEST2_CLASS(StrChar)
	{
		UNITTEST2_METHOD(StrChar)
		{
			UNITTEST2_TRUE(StrChar::IsSpace(' ') == true);
			UNITTEST2_TRUE(StrChar::IsSpace('a') == false);
			UNITTEST2_TRUE(StrChar::IsSpace('\0') == false);

			for (WORD uRadix = StrChar::k_uRadixMin; uRadix <= StrChar::k_uRadixMax; uRadix++)
			{
				for (UINT uVal = 0; uVal < uRadix; uVal++)
				{
					char ch = StrChar::U2Radix(uVal, uRadix);
					UINT uVal2 = StrChar::Radix2U(ch, uRadix);
					UNITTEST2_TRUE(uVal2 == uVal);
				}

				UNITTEST2_TRUE(StrChar::U2Radix(uRadix + 1, uRadix) == '?');
				UNITTEST2_TRUE(StrChar::Radix2U('?', uRadix) == -1);
			}
		}
	};
	UNITTEST2_REGISTER(StrChar, UNITTEST_LEVEL_Core);
}
