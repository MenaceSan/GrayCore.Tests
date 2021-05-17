//
//! @file cAppImpl.Tests.cpp
//
#include "pch.h"
#include "cAppImpl.h"
 
namespace Gray
{
	struct cAppImplTestCmd : public cAppCommand
	{
		const int m_nTestNum;
		cStringF m_sSwitch;		//!< abbreviated -switch or /switch (case sensative) optional, nullptr allowed
		cStringA m_sName;		//!< symbolic name for -switch or /switch (case insensative). MUST be unique.
		bool m_bCalled = false;

		cAppImplTestCmd(int nTestNum)
			: m_nTestNum(nTestNum) 
			, cAppCommand(m_sSwitch = cStringA::GetFormatf("t%d", nTestNum), m_sName = cStringA::GetFormatf("Test%d", nTestNum), nullptr, "Test command")
		{
		}

		virtual HRESULT DoCommand(int iArgN, const FILECHAR_t* pszArg) override 	//!< call this if we see the m_pszCmd switch. can consume more arguments (or not).
		{
			cLogMgr::I().addInfoF("Test command handler %d", m_nTestNum);
			m_bCalled = true;
			return m_nTestNum-1;
		}
	};

	UNITTEST_CLASS(cAppImpl)
	{
		// add some task command handlers.

		UNITTEST_METHOD(cAppImpl)
		{
			bool created = false;
			cAppImpl* pInst = cAppImpl::get_SingleU();
			if (pInst == nullptr)
			{
				pInst = new cAppImpl("TestApp");
				created = true;
				pInst->m_State.InitArgsF("-t1=234234 -t2 123 -t2=234 -t1");
			}
			else
			{
				// Add command calls to the end of the existing command line.
				int i = pInst->m_State.m_Args.get_ArgsQty();

				cAppArgs& argset = pInst->m_State.m_Args;
				argset.m_asArgs.Add("-t1=234234");	// s_t1
				argset.m_asArgs.Add("-t2");	// s_t2
				argset.m_asArgs.Add("123");
				argset.m_asArgs.Add("-t2=234");
				argset.m_asArgs.Add("-t1");

				// argset.m_asArgs.InitArgsLine(pInst->m_State.m_Args.get_ArgsQty(), pInst->m_State.m_Args.get_ArgsStr());
			}


			static cAppImplTestCmd s_t1(1);
			pInst->RegisterCommand(s_t1);
			static cAppImplTestCmd s_t2(2);
			pInst->RegisterCommand(s_t2);
			static cAppImplTestCmd s_t3(3);		// never called.
			pInst->RegisterCommand(s_t3);

			HRESULT hRes = pInst->RunCommands();
			UNITTEST_TRUE(SUCCEEDED(hRes));

			UNITTEST_TRUE(s_t1.m_bCalled);
			UNITTEST_TRUE(s_t2.m_bCalled);
			UNITTEST_TRUE(!s_t3.m_bCalled);
		}
	};

	UNITTEST2_REGISTER(cAppImpl, UNITTEST_LEVEL_Core);
}
