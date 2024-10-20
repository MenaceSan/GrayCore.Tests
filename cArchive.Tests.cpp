//! @file cArchive.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cArchive.h>
#include <GrayCore/include/cStreamQueue.h>
#include <GrayCore/include/cUnitTest.h>

namespace Gray {
struct cUnitTestArchive1 : public cObject {
    // A class with a bunch of types to serialize.
    size_t _nSize;
    int m_i1;
    int m_i2;
    UINT m_u1;
    UINT64 m_u2;
    cStringA m_s1;
    cStringW m_s2;
    cString m_s3;
    // cArrayVal

    cUnitTestArchive1() : _nSize(654321), m_i1(1), m_i2(22), m_u1(333), m_u2(444444), m_s1("test"), m_s2(L"Junk Test String"), m_s3(cUnitTestCur::k_sTextBlob.get_CPtr()) {}

    void SetZero() {
        _nSize = 0;
        m_i1 = 0;
        m_i2 = 0;
        m_u1 = 0;
        m_u2 = 0;
        m_s1.SetErase();
        m_s2.SetErase();
        m_s3.SetErase();
    }

    HRESULT Serialize(cArchive& a) override {
        // Read or write all members of this structure.
        HRESULT hRes;
        hRes = a.SerializeSize(_nSize);
        hRes = a.SerializeT(m_i1);
        hRes = a.SerializeT(m_i2);
        hRes = a.SerializeT(m_u1);
        hRes = a.SerializeT(m_u2);
        hRes = m_s1.Serialize(a);
        hRes = m_s2.Serialize(a);
        hRes = m_s3.Serialize(a);
        return hRes;
    }
    bool IsEqual(const cUnitTestArchive1& x) const {
        if (x._nSize != _nSize) return false;
        if (x.m_i1 != m_i1) return false;
        if (x.m_i2 != m_i2) return false;
        if (x.m_u1 != m_u1) return false;
        if (x.m_u2 != m_u2) return false;
        if (x.m_s1 != m_s1) return false;
        if (x.m_s2 != m_s2) return false;
        if (x.m_s3 != m_s3) return false;
        return true;
    }
};

struct UNITTEST_N(cArchive) : public cUnitTest {
    UNITTEST_METHOD(cArchive) {
        cStreamQueue q1;
        cArchive awrite(q1, true);  // write

        cUnitTestArchive1 t1;
        UNITTEST_TRUE(t1.IsEqual(t1));
        HRESULT hRes = t1.Serialize(awrite);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        size_t nQSize = q1.get_ReadQty();
        const void* pQData = q1.get_ReadPtr();

        cArchive aread(q1, false);  // read it back.

        cUnitTestArchive1 t2;
        t2.SetZero();
        UNITTEST_TRUE(!t1.IsEqual(t2));

        hRes = t2.Serialize(aread);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        UNITTEST_TRUE(t1.IsEqual(t2));

        cStreamQueue q2;
        cArchive awrite2(q2, true);  // write it again
        hRes = t2.Serialize(awrite2);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        // it is the same?
        UNITTEST_TRUE(q2.get_ReadQty() == (int)nQSize);
        const void* pQData2 = q2.get_ReadPtr();
        UNITTEST_TRUE(cMem::IsEqual(pQData2, pQData, nQSize));
    }
};
UNITTEST2_REGISTER(cArchive, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
