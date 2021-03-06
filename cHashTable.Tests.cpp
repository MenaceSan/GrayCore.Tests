// 
//! @file cHashTable.Tests.cpp
//
#include "pch.h"
#include "cHashTable.h"
#include "cUniquePtr.h"

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

	class cUnitTestHashRef : public cRefBase, public cUnitTestHashStruct
	{
	public:
		cUnitTestHashRef(HASHCODE_t nHashCode = 0)
			: cUnitTestHashStruct(nHashCode)
		{
		}
		HASHCODE_t get_HashCode() const	noexcept // resolve ambiguous.
		{
			return cUnitTestHashStruct::get_HashCode();
		}
	};

	UNITTEST_CLASS(cHashTable)
	{
		UNITTEST_METHOD(cHashTable)
		{
			//! @todo add and remove stuff. cUnitTestHashRef

			cHashTableStruct<cUnitTestHashStruct, HASHCODE_t, 5> hashtable1;
			UNITTEST_TRUE(hashtable1.k_HASH_ARRAY_QTY == 32);
			hashtable1.Add(cUnitTestHashStruct(123));
			// UNITTEST_TRUE(hashtable1.FindArgForKey(123) == t1.get_Ptr());

			cHashTableRef<cUnitTestHashRef, HASHCODE_t, 5> hashtable2;
			UNITTEST_TRUE(hashtable1.k_HASH_ARRAY_QTY == 32);
			cRefPtr<cUnitTestHashRef> t2(new cUnitTestHashRef(123));
			hashtable2.Add(t2);
			UNITTEST_TRUE(hashtable2.FindArgForKey(123) == t2);

		}
	};
	UNITTEST2_REGISTER(cHashTable, UNITTEST_LEVEL_Core);
}
