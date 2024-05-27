//! @file cLogMgr.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cLogMgr.h>

namespace Gray {
struct UNITTEST_N(cLogMgr) : public cUnitTest {
    UNITTEST_METHOD(cLogMgr) {
        cLogMgr& logmgr = cLogMgr::I();

        cRefPtr<cLogSinkDebug> pLogDebug(new cLogSinkDebug);
        logmgr.AddSink(pLogDebug);
        UNITTEST_TRUE(cLogSinkDebug::AddSinkCheck(&logmgr) == S_FALSE);

        cLogSink* pSink1 = logmgr.FindSinkType(typeid(cLogSinkDebug));
        UNITTEST_TRUE(pSink1 != nullptr);  // already added

        // This should print twice.
        logmgr.addEventS(LOG_ATTR_DEBUG, LOGLVL_t::_INFO, "Test Event to cLogSinkDebug * 2 (double logged event)" );
        logmgr.RemoveSink(pLogDebug);
    }
};
UNITTEST2_REGISTER(cLogMgr, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
