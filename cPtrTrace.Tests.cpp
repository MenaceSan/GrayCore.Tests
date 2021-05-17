//
//! @file cPtrTrace.Tests.cpp
//
#include "pch.h"
#include "cPtrTrace.h"
#include "cPtrTraceMgr.h"
#include "cLogEvent.h"
#include "cNewPtr.h"

namespace Gray
{
	UNITTEST_CLASS(cPtrTrace)
	{
		void TestNewPtr()
		{
			// Test the problem auto_ptr<> has with hidden transfer/dupe of ownership.
			cNewPtr<BYTE> p1(new BYTE[128]);
			UNITTEST_TRUE(p1.isValidPtr());
			cNewPtr<BYTE> p2;
			UNITTEST_TRUE(p1.isValidPtr());
			UNITTEST_TRUE(!p2.isValidPtr());
			p2 = p1;	// Transfer of ownership ? hidden!?
			UNITTEST_TRUE(!p1.isValidPtr());
			UNITTEST_TRUE(p2.isValidPtr());
			// MUST NOT be destroyed 2 times ?
		}

		UNITTEST_METHOD(cPtrTrace)
		{
			cUnitTests& uts = cUnitTests::I();
			cPtrTraceMgr& traceMgr = cPtrTraceMgr::I();

			TestNewPtr();

			bool bPrevActive = cPtrTrace::sm_bActive;
			ITERATE_t nPrevCount = traceMgr.m_aTraces.GetSize();
			cPtrTrace::sm_bActive = true;

#ifdef USE_IUNK_TRACE
			{
				int iRefCount = 0;
				cIUnkPtr<cLogEvent> p1(new cLogEvent(LOG_ATTR_0, LOGLEV_ANY, "UnitTest", ""));
				IUNK_ATTACH(p1);
				iRefCount = p1.get_RefCount();
				UNITTEST_TRUE(iRefCount == 1);
				cIUnkPtr<cLogEvent> p2(p1);
				IUNK_ATTACH(p2);
				iRefCount = p1.get_RefCount();
				UNITTEST_TRUE(iRefCount == 2);
				traceMgr.TraceDump(*uts.m_pLog, 2 + nPrevCount);
				p2 = nullptr;	// ReleasePtr
				iRefCount = p1.get_RefCount();
				UNITTEST_TRUE(iRefCount == 1);

				cRefBasePtr pBase(new cRefBase());
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
			traceMgr.TraceDump(*uts.m_pLog, nPrevCount);
		}
	};
	UNITTEST2_REGISTER(cPtrTrace, UNITTEST_LEVEL_Core);
}
