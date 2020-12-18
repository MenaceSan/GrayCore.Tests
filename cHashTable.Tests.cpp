// 
//! @file cHashTable.Tests.cpp
//
#include "pch.h"
#include "cHashTable.h"
#include "cNewPtr.h"

namespace Gray
{
	class cUnitTestHashStruct
	{
	private:
		HASHCODE_t m_nHashCode;
	public:
		cUnitTestHashStruct(HASHCODE_t nHashCode = 0)
			: m_nHashCode(nHashCode)
		{
		}
		HASHCODE_t get_HashCode() const noexcept
		{
			return m_nHashCode;
		}
	};

	class cUnitTestHashSmart : public cRefBase, public cUnitTestHashStruct
	{
	public:
		cUnitTestHashSmart(HASHCODE_t nHashCode = 0)
			: cUnitTestHashStruct(nHashCode)
		{
		}
		HASHCODE_t get_HashCode() const	noexcept // resolve ambiguous.
		{
			return cUnitTestHashStruct::get_HashCode();
		}
	};

	UNITTEST2_CLASS(cHashTable)
	{
		UNITTEST2_METHOD(cHashTable)
		{
			//! @todo add and remove stuff. cUnitTestHashSmart

			cHashTableStruct<cUnitTestHashStruct, HASHCODE_t, 5> hashtable1;
			UNITTEST2_TRUE(hashtable1.get_HashArrayQty() == 32);
			hashtable1.Add(cUnitTestHashStruct(123));
			// UNITTEST2_TRUE(hashtable1.FindArgForKey(123) == t1.get_Ptr());

			cHashTableSmart<cUnitTestHashSmart, HASHCODE_t, 5> hashtable2;
			UNITTEST2_TRUE(hashtable1.get_HashArrayQty() == 32);
			cRefPtr<cUnitTestHashSmart> t2(new cUnitTestHashSmart(123));
			hashtable2.Add(t2);
			UNITTEST2_TRUE(hashtable2.FindArgForKey(123) == t2);

		}
	};
	UNITTEST2_REGISTER(cHashTable, UNITTEST_LEVEL_Core);
}
