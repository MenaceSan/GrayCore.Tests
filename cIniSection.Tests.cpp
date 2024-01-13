//
//! @file cIniSection.Tests.cpp
//
#include "pch.h"
#include <GrayCore/include/cIniSection.h>

namespace Gray {
struct UNITTEST_N(cIniSection) : public cUnitTest {
    UNITTEST_METHOD(cIniSection) {
        // Test read a blob of data as a section.
        static const IniChar_t k_SectionData[] =
            "DATA1\r\n"    // 7
            "DATA2\r\n"    // 7
            "DATA3=3\r\n"  // 9
            "DATA4:4\r\n"  // 9
            "\r\n"
            "DATA6:5\r\n"  // ignored.
            "\r\n";

        cIniSectionData section;
        StrLen_t iRead = section.SetLinesParse(k_SectionData, _countof(k_SectionData), nullptr, STRP_START_WHITE | STRP_MERGE_CRNL | STRP_END_WHITE | STRP_EMPTY_STOP);
        UNITTEST_TRUE(iRead == 33);
        UNITTEST_TRUE(section.get_LineQty() == 5);

        static const IniChar_t* k_tSent = "This is a sentence. And another. // comment";
        iRead = cIniReader::FindScriptLineEnd(k_tSent);  // k_tSent
        UNITTEST_TRUE(iRead == 32);
    }
};
UNITTEST2_REGISTER(cIniSection, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
