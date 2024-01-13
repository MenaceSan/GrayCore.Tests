//
//! @file HResult.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/HResult.h>

namespace Gray {
struct UNITTEST_N(HResult) : public cUnitTest {
    UNITTEST_METHOD(HResult) {
        HRESULT hRes = HResult::Make(8, (FACILITY_TYPE)FACILITY_NULL, 3000);  // test special reserved codes.
        UNITTEST_TRUE(hRes == ((HRESULT)0xc0000bb8));

        HResult::AddCodesDefault();

        // Test
        // AddCodesFile()

        static const HRESULT k_HResCodes[] = {
            (HRESULT)(0x080072ee2),  // WinINet code. 12002
            E_FAIL,
            E_INVALIDARG,
            MAKE_HRESULT(1, FACILITY_POSIX, 22),
            (HRESULT)(0x81111111),  // junk code.
        };
        for (size_t i = 0; i < _countof(k_HResCodes); i++) {
            // HResult::GetTextV
            HRESULT hResTest = k_HResCodes[i];
            cString sError = cString::GetErrorStr(hResTest);
            UNITTEST_TRUE(sError.GetLength() > 0);
            // Reversible?
            HRESULT hResRet = HResult::GetHResFromStr(sError, sError.GetLength());
            UNITTEST_TRUE(hResRet == hResTest);
        }
    }
};
UNITTEST2_REGISTER(HResult, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
