//
// cLogMgr.Tests.cpp
//
#include "pch.h"
#include "cLogMgr.h"

namespace Gray
{
	UNITTEST2_CLASS(cLogMgr)
	{
		UNITTEST2_METHOD(cLogMgr)
		{
			cLogMgr& logmgr = cLogMgr::I();

			cRefPtr<cLogAppendDebug> pLogDebug(new cLogAppendDebug);
			logmgr.AddAppender(pLogDebug);
			UNITTEST2_TRUE(cLogAppendDebug::AddAppenderCheck(&logmgr) == S_FALSE);

			cLogAppender* pAppender1 = logmgr.FindAppenderType(typeid(cLogAppendDebug));
			UNITTEST2_TRUE(pAppender1 != nullptr);	// already added

			// This should print twice.
			logmgr.addEventS(LOG_ATTR_DEBUG, LOGLEV_INFO, "Test Event to cLogAppendDebug * 2 (double logged event)", "");
			logmgr.RemoveAppender(pLogDebug);
		}
	};
	UNITTEST2_REGISTER(cLogMgr, UNITTEST_LEVEL_Core);
}

