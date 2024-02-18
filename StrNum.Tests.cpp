//! @file StrNum.Tests.cpp
#include "pch.h"
#include <GrayCore/include/StrBuilder.h>
#include <GrayCore/include/StrNum.h>
#include <GrayCore/include/cTypes.h>

namespace Gray {
struct UNITTEST_N(StrNum) : public cUnitTest {
    void TestDoubleStr(double dVal1, const char* pszVal) {
        // Test MUST be reversible. AND do same string to double convert as C++ compiler.
        // Test 1 - Simple reversal of a string.
        double dVal2 = StrNum::toDouble(pszVal, nullptr);
        UNITTEST_TRUE(dVal2 == dVal1);  // NOT 123.12299999

        char szTmp[StrNum::k_LEN_MAX_DIGITS];
        StrLen_t nLenRet = StrNum::DtoAG2(dVal2, szTmp);  // not clipped decimal.
        UNITTEST_TRUE(nLenRet > 0);
        UNITTEST_TRUE(!StrT::Cmp(szTmp, pszVal));

        StrBuilder<char> sb;
        sb.AddFloat(dVal1);
        UNITTEST_TRUE(!StrT::Cmp(szTmp, sb.get_CPtr()));
    }

    template <typename _TYPE>
    void TestArray() {
        _TYPE aVals[4];
        size_t nRet = StrNum::ToValArray<_TYPE>(TOSPAN(aVals), "1,2,3,4");
        UNITTEST_TRUE(nRet == 4);

        StrBuilder<char> sb;
        sb.AddCSV<_TYPE>(TOSPAN(aVals));  // static
        UNITTEST_TRUE(sb.get_Length() == 7);
    }

    static StrLen_t GRAYCALL DToATestLegacy(double dVal, cSpanX<char>& ret, StrLen_t iDecPlaces) {
        // Used for compatibility testing.
        char szFormat[64];
        szFormat[0] = '%';
        StrLen_t iLenFormat = 1;
        if (iDecPlaces >= 0) {
            szFormat[iLenFormat++] = '.';
            iLenFormat += StrT::UtoA(iDecPlaces, ToSpan(szFormat + iLenFormat, STRMAX(szFormat) - iLenFormat), 10);
        }
        szFormat[iLenFormat++] = 'f';  // will not produce E notation.
        szFormat[iLenFormat] = '\0';

        StrLen_t iLen = StrT::sprintfN<char>(ret, szFormat, dVal);  // %.2f
                                         
        // Trim zeros after the .
        char* pszOut = ret.get_DataWork();
        if (StrT::HasChar<char>(pszOut, '.')) {
            iLen = StrNum::GetTrimCharsLen(ToSpan(pszOut, iLen), '0');
            if (iLen > 0 && pszOut[iLen - 1] == '.') {  // get rid of the dot too.
                iLen--;
            }
            pszOut[iLen] = '\0';                               // eat it.
            if (iLen == 1 && pszOut[0] == '0' && dVal != 0) {  // similar effect to %g
                iLen = StrT::sprintfN<char>(ret, CSTRCONST("%e"), dVal);
                // Trim 0 before e
                StrLen_t iLenE = StrT::FindCharN<char>(pszOut, 'e');
                if (iLenE > 0) {
                    StrLen_t iLenE2 = StrNum::GetTrimCharsLen(ToSpan(pszOut, iLenE), '0');
                    if (iLenE > iLenE2) {
                        // e.g. "6.667000e-20"
                        cMem::CopyOverlap(pszOut + iLenE2, pszOut + iLenE, ((iLen + 1) - iLenE) * sizeof(char));
                    }
                }
            }
        }
        return iLen;
    }

    void TestTypeRounding() {
        // Test convert to float from double.
        float f1 = 1.5f;  // same accuracy
        double dVal = 1.5;
        float f2 = (float)dVal;
        UNITTEST_TRUE(f2 == f1);
        UNITTEST_TRUE(dVal == f1);

        dVal = 1.23;  // more accurate
        f1 = 1.23f;
        f2 = (float)dVal;
        UNITTEST_TRUE(f2 == f1);
        UNITTEST_TRUE(dVal != f1);

        dVal = 1.38;
        f1 = 1.38f;  // more accurate
        f2 = (float)dVal;
        UNITTEST_TRUE(f2 == f1);
        UNITTEST_TRUE(dVal != f1);
    }

    void TestExtremes() {
        // test extremes.
        char szTmp[StrNum::k_LEN_MAX_DIGITS];

        double dVal = -0.00000000000001;
        StrLen_t nLenRet = StrNum::DtoAG2(dVal, szTmp);  // "-0" ?
        UNITTEST_TRUE(!StrT::Cmp<char>(szTmp, "-1e-14"));
        UNITTEST_TRUE(nLenRet == 6);
        dVal = StrNum::toDouble(szTmp);
        UNITTEST_TRUE(dVal == -0.00000000000001);

        // DBL_MAX

        // way too much precision.
        dVal = StrNum::toDouble("123.1234567890123456789012345678901234567890123456789012345678901234567890123456789", nullptr);
        UNITTEST_TRUE(dVal == 123.12345678901235);

        dVal = 123.1234567890123456789012345678901234567890123456789012345678901234567890123456789;
        UNITTEST_TRUE(dVal == 123.12345678901235);

        // Test Large Exponent.
        dVal = cTypeLimit<double>::Max();
        nLenRet = StrNum::DtoAG2(dVal, szTmp);
        UNITTEST_TRUE(!StrT::Cmp<char>(szTmp, "1.7976931348623157e+308"));

        dVal = cTypeLimit<double>::Min();
        nLenRet = StrNum::DtoAG2(dVal, szTmp);
        UNITTEST_TRUE(!StrT::Cmp<char>(szTmp, "-1.7976931348623157e+308"));
    }

    void TestDecimalPlaces() {
        char szTmp[StrNum::k_LEN_MAX_DIGITS];
        StrLen_t nLenRet = StrNum::DtoAG2(0.125, szTmp, -1, '\0');  // Test clip decimal places.
        UNITTEST_TRUE(!StrT::Cmp(szTmp, "0.125"));

        nLenRet = StrNum::DtoAG2(123.123, szTmp, 2, '\0');  // Test clip decimal places.
        UNITTEST_TRUE(!StrT::Cmp(szTmp, "123.12"));

        nLenRet = StrNum::DtoAG2(99.99999, szTmp, 2, '\0');  // Test clip decimal places.
        UNITTEST_TRUE(!StrT::Cmp(szTmp, "100.00"));

        double dVal = StrNum::toDouble("123.1234567", nullptr);
        UNITTEST_TRUE(dVal == 123.1234567);

        nLenRet = StrNum::DtoAG2(dVal, szTmp, 6, '\0');         // clip.
        UNITTEST_TRUE(!StrT::Cmp<char>(szTmp, "123.123457"));   // rounds last digit up.
        nLenRet = StrNum::DtoAG2(dVal, szTmp, 7, '\0');         // clip does nothing.
        UNITTEST_TRUE(!StrT::Cmp<char>(szTmp, "123.1234567"));  // NO round last digit.
    }

    void TestExp() {
        // Test 3 - Exponent.
        double dVal = StrNum::toDouble("3.141600E+000", nullptr);  // dVal = 3.1415999999999999
        dVal -= 3.1415;
        UNITTEST_TRUE(dVal <= 0.0001);  // near

        char szTmp[StrNum::k_LEN_MAX_DIGITS];
        StrLen_t nLenRet = StrT::DtoA<char>(dVal, TOSPAN(szTmp));
        // UNITTEST_TRUE(!StrT::Cmp(szTmp, "123.123"));

        // Test Exponent sign,
        dVal = StrNum::toDouble("-1.797693e-100");
        UNITTEST_TRUE(dVal > -1 && dVal < 0);
        nLenRet = StrNum::DtoAG2(dVal, szTmp);
        UNITTEST_TRUE(!StrT::Cmp<char>(szTmp, "-1.7976929999999997e-100"));  // ???
    }

    UNITTEST_METHOD(StrNum) {
        // Test float,double.
        // Test 1 - Simple reversal.
        // Accurately storing floats as strings is tricky. Some numbers are represented better than others.

        TestDoubleStr(35.913279, "35.913279");  // this value cannot be represented exactly! = 35.913279000000003

        TestDoubleStr(0, "0");
        TestDoubleStr(123.123, "123.123");
        TestDoubleStr(-1e-14, "-1e-14");
        // TestDoubleStr(6.667e-20, "6.667e-20");

        // Test formats.
        double dVal = StrNum::toDouble(".123", nullptr);
        dVal -= 0.123;
        UNITTEST_TRUE(dVal <= 0.0001);

        TestExp();
        TestDecimalPlaces();
        TestExtremes();
        TestTypeRounding();
        TestArray<double>();
        TestArray<int>();
    }
};
UNITTEST2_REGISTER(StrNum, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
