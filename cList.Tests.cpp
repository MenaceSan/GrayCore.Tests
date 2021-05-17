// 
//! @file cList.Tests.cpp
//
#include "pch.h"
#include "cList.h"
#include "cListNodeRef.h"

namespace Gray
{
	class cUnitTestListRef : public cListNodeRef < cUnitTestListRef >
	{
	public:
		int m_iVal;
	public:
		cUnitTestListRef(int iVal)
			: m_iVal(iVal)
		{
		}
	};

	UNITTEST_CLASS(cList)
	{
		UNITTEST_METHOD(cList)
		{
			cListT<cUnitTestListRef> list;
			// list.InsertHead(new cUnitTestListRef(1));
			// list.Empty();

		}
	};
	UNITTEST2_REGISTER(cList, UNITTEST_LEVEL_Core);
}
