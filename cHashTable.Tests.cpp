//! @file cHashTable.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cHashTable.h>
#include <GrayCore/include/cUniquePtr.h>

namespace Gray {
class cUnitTestHashStruct {
    typedef cUnitTestHashStruct THIS_t;
    HASHCODE_t m_nHashCode;

 public:
    cUnitTestHashStruct(HASHCODE_t nHashCode = 0) : m_nHashCode(nHashCode) {}
    HASHCODE_t get_HashCode() const noexcept {
        return m_nHashCode;
    }
};

struct cUnitTestHashRef : public cRefBase, public cUnitTestHashStruct {
    cUnitTestHashRef(HASHCODE_t nHashCode = 0) : cUnitTestHashStruct(nHashCode) {}
    HASHCODE_t get_HashCode() const noexcept { // resolve ambiguous.
        return cUnitTestHashStruct::get_HashCode();
    }
};

struct UNITTEST_N(cHashTable) : public cUnitTest {
    UNITTEST_METHOD(cHashTable) {
        //! @todo add and remove stuff. cUnitTestHashRef

        cHashTableStruct<cUnitTestHashStruct, HASHCODE_t, 5> hashtable1;
        UNITTEST_TRUE(hashtable1.k_HASH_BUCKET_QTY == 32);
        hashtable1.Add(cUnitTestHashStruct(123));
        // UNITTEST_TRUE(hashtable1.FindArgForKey(123) == t1.get_Ptr());

        cHashTableRef<cUnitTestHashRef, HASHCODE_t, 5> hashtable2;
        UNITTEST_TRUE(hashtable1.k_HASH_BUCKET_QTY == 32);
        cRefPtr<cUnitTestHashRef> t2(new cUnitTestHashRef(123));
        hashtable2.Add(t2);
        UNITTEST_TRUE(hashtable2.FindArgForKey(123) == t2);
    }
};
UNITTEST2_REGISTER(cHashTable, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
