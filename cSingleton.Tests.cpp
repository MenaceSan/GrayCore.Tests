// 
//! @file cSingleton.Tests.cpp
//
#include "pch.h"
#include "cSingleton.h"

namespace Gray
{
	class cUnitTestSing : public cSingleton < cUnitTestSing >
	{
	public:
		cUnitTestSing() : cSingleton<cUnitTestSing>(this, typeid(cUnitTestSing))
		{
		}
		CHEAPOBJECT_IMPL;
	};

	UNITTEST2_CLASS(cSingleton)
	{
		UNITTEST2_METHOD(cSingleton)
		{
			UNITTEST2_TRUE(!cUnitTestSing::isSingleCreated());
			{
				cUnitTestSing inst;
				UNITTEST2_TRUE(cUnitTestSing::isSingleCreated());
				// Will be destructed at app close.
			}
			UNITTEST2_TRUE(!cUnitTestSing::isSingleCreated());
		}
	};
	UNITTEST2_REGISTER(cSingleton, UNITTEST_LEVEL_Core);
}
