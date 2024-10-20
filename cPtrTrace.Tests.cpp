//! @file cPtrTrace.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cLogEvent.h>
#include <GrayCore/include/cPtrTrace.h>
#include <GrayCore/include/cPtrTraceMgr.h>
#include <GrayCore/include/cUniquePtr.h>

namespace Gray {
struct UNITTEST_N(cPtrTrace) : public cUnitTest {
    void TestUniquePtr() {
        // Test the problem auto_ptr<> has with hidden transfer/dupe of ownership.
        cUniquePtr<BYTE> p1(new BYTE[128]);
        const size_t nSizeTest = sizeof(p1);

        UNITTEST_TRUE(p1.isValidPtr());
        cUniquePtr<BYTE> p2;
        UNITTEST_TRUE(p1.isValidPtr());
        UNITTEST_TRUE(!p2.isValidPtr());
        p2 = p1;  // Transfer of ownership ? hidden!? move()
        UNITTEST_TRUE(!p1.isValidPtr());
        UNITTEST_TRUE(p2.isValidPtr());
        // MUST NOT be destroyed 2 times ?
    }

    void TestIUnkPtr(cUnitTests& uts) {
        const bool bPrevActive = cPtrTrace::sm_bActive;
        cPtrTraceMgr& traceMgr = cPtrTraceMgr::I();
        const ITERATE_t nPrevCount = traceMgr.GetSize();
        cPtrTrace::sm_bActive = true;

        cIUnkPtr<cLogEvent> p0;
        const size_t nSizeTest = sizeof(p0);

#if defined(USE_PTRTRACE_IUNK) || defined(USE_PTRTRACE_REF)
        {
            REFCOUNT_t iRefCount = 0;
            cIUnkPtr<cLogEvent> p1(new cLogEvent(LOG_ATTR_0, LOGLVL_t::_ANY, "UnitTest"));
            IUNK_TRACE(p1);
            iRefCount = p1.get_RefCount();
            UNITTEST_TRUE(iRefCount == 1);
            cIUnkPtr<cLogEvent> p2(p1);
            IUNK_TRACE(p2);
            iRefCount = p1.get_RefCount();
            UNITTEST_TRUE(iRefCount == 2);
            traceMgr.TraceDump(uts._pLog, 2 + nPrevCount);
            p2 = nullptr;  // ReleasePtr
            iRefCount = p1.get_RefCount();
            UNITTEST_TRUE(iRefCount == 1);

            cRefPtr<> pBase(new cLogEvent(LOG_ATTR_0, LOGLVL_t::_ANY, "UnitTest2"));
            UNITTEST_TRUE(pBase != nullptr);

            IUnknown* pObject = nullptr;
            HRESULT hRes = pBase->QueryInterface(__uuidof(IUnknown), (void**)&pObject);
            UNITTEST_TRUE(SUCCEEDED(hRes));
            pObject->Release();
            UNITTEST_TRUE(pObject == pBase);
            iRefCount = pBase.get_RefCount();
            UNITTEST_TRUE(iRefCount == 1);
        }
#endif
        cPtrTrace::sm_bActive = bPrevActive;
        traceMgr.TraceDump(uts._pLog, nPrevCount);
    }

    UNITTEST_METHOD(cPtrTrace) {
        cUnitTests& uts = cUnitTests::I();
        TestUniquePtr();
        TestIUnkPtr(uts);

        cRefPtr<cLogEvent> pr0;
        const size_t nSizeTest = sizeof(pr0);   // 16 for x64 _DEBUG/USE_PTRTRACE_REF, 
        UNITTEST_TRUE(nSizeTest >= sizeof(UINT_PTR));
    }
};
UNITTEST2_REGISTER(cPtrTrace, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
