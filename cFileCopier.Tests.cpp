//
//! @file cFileCopier.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cFileCopier.h>

namespace Gray {
void GRAYCALL UnitTest_CopyTo(IFileCopier* pDst) {
    //! For testing an implementation of IFileCopier
    UNREFERENCED_PARAMETER(pDst);
}
void GRAYCALL UnitTest_CopyFrom(IFileCopier* pSrc) {
    //! For testing an implementation of IFileCopier
    UNREFERENCED_PARAMETER(pSrc);
}
struct UNITTEST_N(cFileCopier) : public cUnitTest {
    UNITTEST_METHOD(cFileCopier) {
        // TODO cFileCopier copy files from one dir to another.
        cUnitTests& uts = cUnitTests::I();
    }
};
UNITTEST2_REGISTER(cFileCopier, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
