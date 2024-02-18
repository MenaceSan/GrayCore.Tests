//
//! @file StrFormat.Tests.cpp
// clang-format off
#include "pch.h"
// clang-format on
#include <GrayCore/include/StrBuilder.h>
#include <GrayCore/include/StrFormat.h>
#include <GrayCore/include/cHeap.h>
#include <GrayCore/include/cIniBase.h>  //  IIniBaseGetter
#include <GrayCore/include/cTypes.h>

namespace Gray {
/// <summary>
/// Argument types for testing printf()
/// </summary>
enum STRFORMATTEST_TYPE {
    STRFORMATTEST_0,  // no args intentionally.
    STRFORMATTEST_null,

    STRFORMATTEST_s1,  // "abc"
    STRFORMATTEST_s2,  // "abcdef"
    STRFORMATTEST_sA,  // "A string"

    STRFORMATTEST_i0,   // 0
    STRFORMATTEST_i1,   // 123123
    STRFORMATTEST_i2,   //
    STRFORMATTEST_i5,   //
    STRFORMATTEST_im2,  //
    STRFORMATTEST_in,   //

    STRFORMATTEST_x1,   //
    STRFORMATTEST_xlg,  //

    STRFORMATTEST_c1,  //
    STRFORMATTEST_c2,  //

    STRFORMATTEST_f0,   // 0.0
    STRFORMATTEST_fp1,  //
    STRFORMATTEST_f2,   //
    STRFORMATTEST_f3,   // 3.1416, 3.1416, 3.1416
    STRFORMATTEST_f100,

    STRFORMATTEST_fn,   // -123.456
    STRFORMATTEST_fnz,  // -1.797693e-100 < epsilon?
    STRFORMATTEST_flg,  // 1.797693e+100
    STRFORMATTEST_fmax,

    STRFORMATTEST_QTY,
};

/// <summary>
/// Test a format string.
/// </summary>
struct StrFormatTest {
    STRFORMATTEST_TYPE m_eArgs;
    const char* m_pszFormat;
    const char* m_pszExpected;
};

template <typename TYPE>
StrLen_t UnitTestFormat1(typename StrFormat<TYPE>::STRFORMAT_t pFormat, cSpanX<TYPE>& ret, const TYPE* pszFormat, int eArgs) {  // static
    // Supply test arguments.
    switch (eArgs) {
        case STRFORMATTEST_0:
            return (*pFormat)(ret, pszFormat);
        case STRFORMATTEST_null:
            return (*pFormat)(ret, pszFormat, nullptr);

        case STRFORMATTEST_s1:
            return (*pFormat)(ret, pszFormat, StrArg<TYPE>("abc"));
        case STRFORMATTEST_s2:
            return (*pFormat)(ret, pszFormat, StrArg<TYPE>("abcdef"));
        case STRFORMATTEST_sA:
            return (*pFormat)(ret, pszFormat, StrArg<TYPE>("A string"));

        case STRFORMATTEST_i0:
            return (*pFormat)(ret, pszFormat, 0);
        case STRFORMATTEST_i1:
            return (*pFormat)(ret, pszFormat, 123123);
        case STRFORMATTEST_i2:
            return (*pFormat)(ret, pszFormat, 5, 10);  // m_bWidthArg
        case STRFORMATTEST_i5:
            return (*pFormat)(ret, pszFormat, 100, 100, 100, 100, 100);

        case STRFORMATTEST_im2:
            return (*pFormat)(ret, pszFormat, 1977, 650000L);

        case STRFORMATTEST_in:
            return (*pFormat)(ret, pszFormat, -200, 5);

        case STRFORMATTEST_x1:
            return (*pFormat)(ret, pszFormat, 0xA3);
        case STRFORMATTEST_xlg:
            return (*pFormat)(ret, pszFormat, (INT64)0x123ABCL);

        case STRFORMATTEST_c1:
            return (*pFormat)(ret, pszFormat, 'z');
        case STRFORMATTEST_c2:
            return (*pFormat)(ret, pszFormat, 'a', 65);

        case STRFORMATTEST_f0:
            return (*pFormat)(ret, pszFormat, 0.0);
        case STRFORMATTEST_fp1:
            return (*pFormat)(ret, pszFormat, 0.123);
        case STRFORMATTEST_f2:
            return (*pFormat)(ret, pszFormat, 1.123456789012345);
        case STRFORMATTEST_f3:
            return (*pFormat)(ret, pszFormat, 3.1416, 3.1416, 3.1416);
        case STRFORMATTEST_f100:
            return (*pFormat)(ret, pszFormat, 100.0);

        case STRFORMATTEST_fn:
            return (*pFormat)(ret, pszFormat, -123.456);
        case STRFORMATTEST_fnz:
            return (*pFormat)(ret, pszFormat, -1.797693e-100);
        case STRFORMATTEST_flg:
            return (*pFormat)(ret, pszFormat, 1.797693e+100);
        case STRFORMATTEST_fmax:
            return (*pFormat)(ret, pszFormat, cTypeLimit<double>::Max());

            // NaN, Inf ?

            // case STRFORMATTEST_b1:
            //	return (*pFormat)(ret, pszFormat, 0x550F);
        default:
            break;
    }

    ASSERT(0);
    return 0;
}

template <typename TYPE>
void GRAYCALL UnitTestFormatX(typename StrFormat<TYPE>::STRFORMAT_t pFormat, cSpanX<TYPE>& ret, StrLen_t nLenAlloc) {  // static
    //! @note ASSUME sprintf() will '\0' terminate the far end even if it isn't needed? (or overflows)
    //! @arg nLenOut = includes room for '\0'
    //! https://www.cs.fsu.edu/~myers/c++/notes/c_io.html

    // clang-format off
	static const StrFormatTest k_UnitTestData[] = {
		{ STRFORMATTEST_f3,		"floats: %4.2f %+.0e %E \n", "floats: 3.14 +3e+00 3.141600E+00 \n" },

		// Test strings via "%s".
		{ STRFORMATTEST_0,		"%%",					"%" },				//
		{ STRFORMATTEST_0,		"test%%test%%",			"test%test%" },		// 

		{ STRFORMATTEST_null,	"%s",				"(null)" },		// 
		{ STRFORMATTEST_null,	"test%stest",		"test(null)test" },		// 

		{ STRFORMATTEST_s1,		"%s",					"abc" },			// "abc"
		{ STRFORMATTEST_s1,		"%-4s",					"abc " },		// abc
		{ STRFORMATTEST_s1,		"%4s",					" abc" },

		{ STRFORMATTEST_s2,		"%s",					"abcdef" },			// "abcdef"
		{ STRFORMATTEST_s2,		"test%stest",			"testabcdeftest" },
		{ STRFORMATTEST_s2,		"test%.5stest",			"testabcdetest" },
		{ STRFORMATTEST_s2,		"test%4.1stest",		"test   atest" },		// "abcdef"

		{ STRFORMATTEST_s2,		"%4s",					"abcdef" },	// overflow.
		{ STRFORMATTEST_s2,		"%-4s",					"abcdef" },	// abcdef
		{ STRFORMATTEST_s2,		"%.4s",					"abcd" },		// abcdef clipped

		{ STRFORMATTEST_sA,		"%s \n",				"A string \n" },

		// Test Numeric "%d"	
		{ STRFORMATTEST_i1,		"%d",					"123123" },
		{ STRFORMATTEST_i1,		"%2d",					"123123" },	// overflow.
		{ STRFORMATTEST_i1,		"%+2d",					"+123123" },	// overflow.
		{ STRFORMATTEST_in,		"%+2d",					"-200" },	// 
		{ STRFORMATTEST_i0,		"%+2d",					"+0" },	// 

		{ STRFORMATTEST_im2,	"Decimals: %d %ld\n",	"Decimals: 1977 650000\n" },
		{ STRFORMATTEST_im2,	"Preceding with zeros: %010d \n",	"Preceding with zeros: 0000001977 \n" },
		{ STRFORMATTEST_im2,	"Preceding with blanks: %10d \n",	"Preceding with blanks:       1977 \n" },

		{ STRFORMATTEST_im2,	"%-6u",								"1977  " },
		{ STRFORMATTEST_i2,		"Width trick: %*d \n",		"Width trick:    10 \n" },
		{ STRFORMATTEST_in,		"%6d,%3d%%",				"  -200,  5%" },

		// Test Numeric other radix Types "%u" "%x"

		{ STRFORMATTEST_x1,		"%x",		"a3" },
		{ STRFORMATTEST_x1,		"%X",		"A3" },
		{ STRFORMATTEST_x1,		"%04x",		"00a3" },

		{ STRFORMATTEST_xlg,	"%08lX",	"00123ABC" },		// NOTE: "%08LX" will fail.
		{ STRFORMATTEST_i5,		"Some different radices: %u %x %o %#x %#o \n",	"Some different radices: 100 64 144 0x64 0144 \n" },

		// Tests char types. "%c"
		{ STRFORMATTEST_c1,		"%c",					"z" },
		{ STRFORMATTEST_c2,		"Characters: %c %c \n",	"Characters: a A \n" },	// 'a', 65

		// Test floats. "%g" "%f" "%e"

		{ STRFORMATTEST_f0,		"%g",					"0" },
		{ STRFORMATTEST_f0,		"%+g",					"+0" },
		{ STRFORMATTEST_fn,		"%+g",					"-123.456" },

		{ STRFORMATTEST_fp1,	"%g",					"0.123" },
		{ STRFORMATTEST_fp1,	"%.17g",				"0.123" },
		{ STRFORMATTEST_f2,		"%g",					"1.12346" },
		{ STRFORMATTEST_f2,		"%.17g",				"1.123456789012345" },
		{ STRFORMATTEST_f2,		"%.7g",					"1.123457" },		// rounds up last digit.
		{ STRFORMATTEST_f100,	"%g",					"100" },
		{ STRFORMATTEST_fmax,   "%g",					"1.79769e+308" },			// 

		{ STRFORMATTEST_f0,		"%f",					"0.000000" },
		{ STRFORMATTEST_f3,		"%f",					"3.141600" },		// padded to 6 places.
		{ STRFORMATTEST_f3,		"%.2f",					"3.14" },

		{ STRFORMATTEST_f3,		"%f %e %g",				"3.141600 3.141600e+00 3.1416" },
		{ STRFORMATTEST_f3,		"floats: %4.2f %+.0e %E \n", "floats: 3.14 +3e+00 3.141600E+00 \n" },
		{ STRFORMATTEST_f100,	"%f",					"100.000000" },
		{ STRFORMATTEST_f3,		"%9.2f",				"     3.14" },
		{ STRFORMATTEST_f3,		"%-9.2f",				"3.14     " },
		{ STRFORMATTEST_fn,		"%-9.2f",				"-123.46  " },

		{ STRFORMATTEST_fnz,	"Tiny floats: %f",		"Tiny floats: -0.000000" },		//  -1.797693e-100  
		// { STRFORMATTEST_flg,	"Huge floats: %f",		"Huge floats: 17976930000000001340565929873801353323097574629953369710322764223194088032032001524837302450048204800.000000" },		//  1.797693e+100 
		// { STRFORMATTEST_fmax,   "%f",		""	},			// Max float: (316 digits) (even though it has 308 exp?) has ".000000" at the end.
		// { STRFORMATTEST_fmax,   "%.99f",		"" },		// This will crash?! > MAX ?

		{ STRFORMATTEST_f0,		"%e",					"0.000000e+00" },
		{ STRFORMATTEST_fnz,	"%e",					"-1.797693e-100" },
		{ STRFORMATTEST_fnz,	"%.2e",					"-1.80e-100" },
		{ STRFORMATTEST_f3,		"%e",					"3.141600e+00" },
		{ STRFORMATTEST_flg,	"%e",					"1.797693e+100" }, //  1.797693e+100
		{ STRFORMATTEST_fmax,	"%.2e",					"1.80e+308" },
		{ STRFORMATTEST_fmax,   "%e",					"1.797693e+308" },			// 

		// { STRFORMATTEST_b1,		"%016b",	"0101010100001111" },	// 0x550F. NOT valid for M$
	};
    // clang-format on

    for (size_t i = 0; i < _countof(k_UnitTestData); i++) {
        const StrFormatTest& test = k_UnitTestData[i];

        cStringT<TYPE> sFormat = test.m_pszFormat;
        cStringT<TYPE> sExpected = test.m_pszExpected;

        const StrLen_t nLenExp = cValT::Min(sExpected.GetLength(), ret.GetSize() - 1);
        UNITTEST_TRUE(nLenExp < nLenAlloc - 1);

        if (!ret.isNull()) {
            cMem::Fill(ret.get_DataWork(), nLenAlloc * sizeof(TYPE), cHeap::kFillUnusedStack);  // fill background.
        }

        StrLen_t nLenRet = UnitTestFormat1<TYPE>(pFormat, ret, sFormat, test.m_eArgs);
        if (nLenRet == -1) {
            // M$ version will return this if the nLenOut size is too small.
            UNITTEST_TRUE(2 == nLenExp);
            nLenRet = nLenExp;
        } else {
            UNITTEST_TRUE(nLenRet == nLenExp);
        }

        if (!ret.isNull()) {
            // NOTE: M$ vsnprintf() will fill unused data in _DEBUG mode. with 0xFE
            TYPE* pszOut = ret.get_DataWork();
            UNITTEST_TRUE(pszOut[nLenExp] == '\0');
            UNITTEST_TRUE(!StrT::CmpN<TYPE>(pszOut, sExpected, nLenExp));
            BYTE chEnd2 = (BYTE)pszOut[nLenAlloc - 1];
            UNITTEST_TRUE(chEnd2 == cHeap::kFillUnusedStack);
        }
    }
}

template <typename TYPE>
void GRAYCALL UnitTestFormat(typename StrFormat<TYPE>::STRFORMAT_t pFormat) { // static
    // http://www.cplusplus.com/reference/cstdio/printf/
    // pFormat = StrT::sprintfN<TYPE>

    UNITTEST_TRUE(pFormat != nullptr);

    TYPE szTmp[StrT::k_LEN_Default];
    UnitTestFormatX(pFormat, TOSPAN(szTmp), _countof(szTmp));
    UnitTestFormatX(pFormat, ToSpan(szTmp, 3), _countof(szTmp));  // Test buffer size overflows/truncation.

#if 0
    UnitTestFormatX(pFormat, cBlob::k_EmptyBlob);                     // Test size estimation.

	//*****************************
	// Support C# style string {0} ?
	// nLenRet = (*pFormat)(TOSPAN(szTmp), CSTRCONST("test{0}test"), StrArg<TYPE>("fedcba"));
	// UNITTEST_TRUE(nLenRet == 11);	

	// bad pointer. (throw exception?)
	nLenRet = (*pFormat)(TOSPAN(szTmp), CSTRCONST("test%stest"), 12312323);
	UNITTEST_TRUE(nLenRet == 11);
#endif
}

struct UNITTEST_N(StrFormat) : public cUnitTest {
    void TestTemplateBlocks() {
        // StrA::ReplaceTemplateBlock
        struct cUnitTestBlock : public IIniBaseGetter {
            HRESULT PropGet(const char* pszPropTag, OUT cStringI& rsVal) const override {
                if (!Gray::StrT::CmpI(pszPropTag, "blocks")) {
                    rsVal = "TESTBLOCK";
                    return S_OK;
                }
                if (!Gray::StrT::CmpI(pszPropTag, "blo1")) {
                    rsVal = "blo";
                    return S_OK;
                }
                if (!Gray::StrT::CmpI(pszPropTag, "cks2")) {
                    rsVal = "cks";
                    return S_OK;
                }
                return HRESULT_WIN32_C(ERROR_UNKNOWN_PROPERTY);
            }
        } testBlock;

        // recursive test.

        StrBuilder<> s1;
        StrLen_t iRet1 = StrTemplate::ReplaceTemplateBlock(s1, "Test with recursive <?<?blo1?><?cks2?>?>. and junk <?IntentionalUnknownProperty?>.", &testBlock, false);
        UNITTEST_TRUE(iRet1 == 82);
        UNITTEST_TRUE(s1.get_Length() == 71);
        UNITTEST_TRUE(StrT::Cmp(s1.get_CPtr(), "Test with recursive TESTBLOCK. and junk <?IntentionalUnknownProperty?>.") == 0);

        StrBuilder<> s2;
        StrLen_t iRet2 = StrTemplate::ReplaceTemplateBlock(s2, "Test input string with <?blocks?>. And another <?blocks?>.", &testBlock, false);
        UNITTEST_TRUE(iRet2 == 58);
        UNITTEST_TRUE(s2.get_Length() == 56);
        UNITTEST_TRUE(StrT::Cmp(s2.get_CPtr(), "Test input string with TESTBLOCK. And another TESTBLOCK.") == 0);
    }

    void TestBuilder() {
        // test StrBuilder

        StrBuilder<> s1;
        s1.AddFormat("%s - %d - %s - %s", "sdf", 123, "ABC", "XYZ", "IGNOREDJUNK");
        UNITTEST_TRUE(s1.get_Length() == 21);
        UNITTEST_TRUE(StrT::Cmp(s1.get_CPtr(), "sdf - 123 - ABC - XYZ") == 0);
    }

    UNITTEST_METHOD(StrFormat) {
        cTimePerf timeMark1(true);
        UnitTestFormat<wchar_t>(StrFormat<wchar_t>::F);
        UnitTestFormat<char>(StrFormat<char>::F);

        cTimePerf timeMark2(true);
        UnitTestFormat<wchar_t>(StrT::sprintfN<wchar_t>);
        UnitTestFormat<char>(StrT::sprintfN<char>);

        cTimePerf timeMark3(true);
        TIMEPERF_t iDff1 = timeMark1.GetAgeDiff(timeMark2);
        TIMEPERF_t iDff2 = timeMark2.GetAgeDiff(timeMark3);
        UNITTEST_TRUE(iDff1 > 0);
        UNITTEST_TRUE(iDff2 > 0);
        // UNITTEST_TRUE(iDff1 <= iDff2);	// Assume my version is faster.

        TestTemplateBlocks();
        TestBuilder();
    }
};
UNITTEST2_REGISTER(StrFormat, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
