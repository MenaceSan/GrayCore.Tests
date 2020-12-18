//
//! @file cArraySortString.Tests.cpp
//
#include "pch.h"
#include "cArraySortString.h"

namespace Gray
{
	UNITTEST2_CLASS(cArraySortString)
	{
 
		UNITTEST2_METHOD(cArraySortString)
		{
			//! Test sorted arrays. pick a simple template.

			cArrayString<> arrayUns;
			UNITTEST2_TRUE(arrayUns.GetSize() == 0);

			cArraySortStringA array1;
			for (ITERATE_t i = 0; !cUnitTest::k_asTextLines[i].isNull(); i++)
			{
				array1.AddStr(cUnitTest::k_asTextLines[i]);
			}
			UNITTEST2_TRUE(array1.GetSize() == cUnitTest::k_TEXTLINES_QTY);

			StrLen_t iLength = 0;
			GRAY_FOREACH(cString, sVal, array1)
			{
				iLength += sVal.GetLength();
			}
			UNITTEST2_TRUE(iLength >= 66);

			cStringA* ppData = array1.GetData();
			UNITTEST2_TRUE(ppData != nullptr);

			UNITTEST2_TRUE(array1.isArraySorted());
		}
	};

	UNITTEST2_REGISTER(cArraySortString, UNITTEST_LEVEL_Core);
}
