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
enum class STRFT_t {
    _0,  // no args intentionally.
    _null,

    _s1,  // "abc"
    _s2,  // "abcdef"
    _sA,  // "A string"

    _i0,   // 0
    _i1,   // 123123
    _i2,   //
    _i5,   //
    _im2,  //
    _in,   //

    _x1,   //
    _xlg,  //

    _c1,  //
    _c2,  //

    _f0,   // 0.0
    _fp1,  //
    _f2,   //
    _f3,   // 3.1416, 3.1416, 3.1416
    _f100,

    _fn,   // -123.456
    _fnz,  // -1.797693e-100 < epsilon?
    _flg,  // 1.797693e+100
    _fmax,

    _QTY,
};

/// <summary>
/// Test a format string.
/// </summary>
struct StrFormatTest {
    STRFT_t m_eArgs;
    const char* m_pszFormat;
    const char* m_pszExpected;
};

template <typename TYPE>
StrLen_t UnitTestFormat1(typename StrFormat<TYPE>::STRFORMAT_t pFormat, cSpanX<TYPE>& ret, const TYPE* pszFormat, STRFT_t eArgs) {  // static
    // Supply test arguments.
    switch (eArgs) {
        case STRFT_t::_0:
            return (*pFormat)(ret, pszFormat);
        case STRFT_t::_null:
            return (*pFormat)(ret, pszFormat, nullptr);

        case STRFT_t::_s1:
            return (*pFormat)(ret, pszFormat, StrArg<TYPE>("abc"));
        case STRFT_t::_s2:
            return (*pFormat)(ret, pszFormat, StrArg<TYPE>("abcdef"));
        case STRFT_t::_sA:
            return (*pFormat)(ret, pszFormat, StrArg<TYPE>("A string"));

        case STRFT_t::_i0:
            return (*pFormat)(ret, pszFormat, 0);
        case STRFT_t::_i1:
            return (*pFormat)(ret, pszFormat, 123123);
        case STRFT_t::_i2:
            return (*pFormat)(ret, pszFormat, 5, 10);  // _isWidthArg
        case STRFT_t::_i5:
            return (*pFormat)(ret, pszFormat, 100, 100, 100, 100, 100);

        case STRFT_t::_im2:
            return (*pFormat)(ret, pszFormat, 1977, 650000L);

        case STRFT_t::_in:
            return (*pFormat)(ret, pszFormat, -200, 5);

        case STRFT_t::_x1:
            return (*pFormat)(ret, pszFormat, 0xA3);
        case STRFT_t::_xlg:
            return (*pFormat)(ret, pszFormat, (INT64)0x123ABCL);

        case STRFT_t::_c1:
            return (*pFormat)(ret, pszFormat, 'z');
        case STRFT_t::_c2:
            return (*pFormat)(ret, pszFormat, 'a', 65);

        case STRFT_t::_f0:
            return (*pFormat)(ret, pszFormat, 0.0);
        case STRFT_t::_fp1:
            return (*pFormat)(ret, pszFormat, 0.123);
        case STRFT_t::_f2:
            return (*pFormat)(ret, pszFormat, 1.123456789012345);
        case STRFT_t::_f3:
            return (*pFormat)(ret, pszFormat, 3.1416, 3.1416, 3.1416);
        case STRFT_t::_f100:
            return (*pFormat)(ret, pszFormat, 100.0);

        case STRFT_t::_fn:
            return (*pFormat)(ret, pszFormat, -123.456);
        case STRFT_t::_fnz:
            return (*pFormat)(ret, pszFormat, -1.797693e-100);
        case STRFT_t::_flg:
            return (*pFormat)(ret, pszFormat, 1.797693e+100);
        case STRFT_t::_fmax:
            return (*pFormat)(ret, pszFormat, cTypeLimit<double>::Max());

            // NaN, Inf ?

            // case STRFT_t::_b1:
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
		{ STRFT_t::_f3,		"floats: %4.2f %+.0e %E \n", "floats: 3.14 +3e+00 3.141600E+00 \n" },

		// Test strings via "%s".
		{ STRFT_t::_0,		"%%",					"%" },				//
		{ STRFT_t::_0,		"test%%test%%",			"test%test%" },		// 

		{ STRFT_t::_null,	"%s",				"(null)" },		// 
		{ STRFT_t::_null,	"test%stest",		"test(null)test" },		// 

		{ STRFT_t::_s1,		"%s",					"abc" },			// "abc"
		{ STRFT_t::_s1,		"%-4s",					"abc " },		// abc
		{ STRFT_t::_s1,		"%4s",					" abc" },

		{ STRFT_t::_s2,		"%s",					"abcdef" },			// "abcdef"
		{ STRFT_t::_s2,		"test%stest",			"testabcdeftest" },
		{ STRFT_t::_s2,		"test%.5stest",			"testabcdetest" },
		{ STRFT_t::_s2,		"test%4.1stest",		"test   atest" },		// "abcdef"

		{ STRFT_t::_s2,		"%4s",					"abcdef" },	// overflow.
		{ STRFT_t::_s2,		"%-4s",					"abcdef" },	// abcdef
		{ STRFT_t::_s2,		"%.4s",					"abcd" },		// abcdef clipped

		{ STRFT_t::_sA,		"%s \n",				"A string \n" },

		// Test Numeric "%d"	
		{ STRFT_t::_i1,		"%d",					"123123" },
		{ STRFT_t::_i1,		"%2d",					"123123" },	// overflow.
		{ STRFT_t::_i1,		"%+2d",					"+123123" },	// overflow.
		{ STRFT_t::_in,		"%+2d",					"-200" },	// 
		{ STRFT_t::_i0,		"%+2d",					"+0" },	// 

		{ STRFT_t::_im2,	"Decimals: %d %ld\n",	"Decimals: 1977 650000\n" },
		{ STRFT_t::_im2,	"Preceding with zeros: %010d \n",	"Preceding with zeros: 0000001977 \n" },
		{ STRFT_t::_im2,	"Preceding with blanks: %10d \n",	"Preceding with blanks:       1977 \n" },

		{ STRFT_t::_im2,	"%-6u",								"1977  " },
		{ STRFT_t::_i2,		"Width trick: %*d \n",		"Width trick:    10 \n" },
		{ STRFT_t::_in,		"%6d,%3d%%",				"  -200,  5%" },

		// Test Numeric other radix Types "%u" "%x"

		{ STRFT_t::_x1,		"%x",		"a3" },
		{ STRFT_t::_x1,		"%X",		"A3" },
		{ STRFT_t::_x1,		"%04x",		"00a3" },

		{ STRFT_t::_xlg,	"%08lX",	"00123ABC" },		// NOTE: "%08LX" will fail.
		{ STRFT_t::_i5,		"Some different radices: %u %x %o %#x %#o \n",	"Some different radices: 100 64 144 0x64 0144 \n" },

		// Tests char types. "%c"
		{ STRFT_t::_c1,		"%c",					"z" },
		{ STRFT_t::_c2,		"Characters: %c %c \n",	"Characters: a A \n" },	// 'a', 65

		// Test floats. "%g" "%f" "%e"

		{ STRFT_t::_f0,		"%g",					"0" },
		{ STRFT_t::_f0,		"%+g",					"+0" },
		{ STRFT_t::_fn,		"%+g",					"-123.456" },

		{ STRFT_t::_fp1,	"%g",					"0.123" },
		{ STRFT_t::_fp1,	"%.17g",				"0.123" },
		{ STRFT_t::_f2,		"%g",					"1.12346" },
		{ STRFT_t::_f2,		"%.17g",				"1.123456789012345" },
		{ STRFT_t::_f2,		"%.7g",					"1.123457" },		// rounds up last digit.
		{ STRFT_t::_f100,	"%g",					"100" },
		{ STRFT_t::_fmax,   "%g",					"1.79769e+308" },			// 

		{ STRFT_t::_f0,		"%f",					"0.000000" },
		{ STRFT_t::_f3,		"%f",					"3.141600" },		// padded to 6 places.
		{ STRFT_t::_f3,		"%.2f",					"3.14" },

		{ STRFT_t::_f3,		"%f %e %g",				"3.141600 3.141600e+00 3.1416" },
		{ STRFT_t::_f3,		"floats: %4.2f %+.0e %E \n", "floats: 3.14 +3e+00 3.141600E+00 \n" },
		{ STRFT_t::_f100,	"%f",					"100.000000" },
		{ STRFT_t::_f3,		"%9.2f",				"     3.14" },
		{ STRFT_t::_f3,		"%-9.2f",				"3.14     " },
		{ STRFT_t::_fn,		"%-9.2f",				"-123.46  " },

		{ STRFT_t::_fnz,	"Tiny floats: %f",		"Tiny floats: -0.000000" },		//  -1.797693e-100  
		// { STRFT_t::_flg,	"Huge floats: %f",		"Huge floats: 17976930000000001340565929873801353323097574629953369710322764223194088032032001524837302450048204800.000000" },		//  1.797693e+100 
		// { STRFT_t::_fmax,   "%f",		""	},			// Max float: (316 digits) (even though it has 308 exp?) has ".000000" at the end.
		// { STRFT_t::_fmax,   "%.99f",		"" },		// This will crash?! > MAX ?

		{ STRFT_t::_f0,		"%e",					"0.000000e+00" },
		{ STRFT_t::_fnz,	"%e",					"-1.797693e-100" },
		{ STRFT_t::_fnz,	"%.2e",					"-1.80e-100" },
		{ STRFT_t::_f3,		"%e",					"3.141600e+00" },
		{ STRFT_t::_flg,	"%e",					"1.797693e+100" }, //  1.797693e+100
		{ STRFT_t::_fmax,	"%.2e",					"1.80e+308" },
		{ STRFT_t::_fmax,   "%e",					"1.797693e+308" },			// 

		// { STRFT_t::_b1,		"%016b",	"0101010100001111" },	// 0x550F. NOT valid for M$
	};
    // clang-format on

    for (const StrFormatTest& test : k_UnitTestData) {

        cStringT<TYPE> sFormat = test.m_pszFormat;
        cStringT<TYPE> sExpected = test.m_pszExpected;

        const StrLen_t nLenExp = cValT::Min(sExpected.GetLength(), ret.GetSize() - 1);
        UNITTEST_TRUE(nLenExp < nLenAlloc - 1);

        if (!ret.isNull()) {
            cMem::Fill(ret.get_PtrWork(), nLenAlloc * sizeof(TYPE), cHeap::kFillUnusedStack);  // fill background.
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
            TYPE* pszOut = ret.get_PtrWork();
            UNITTEST_TRUE(pszOut[nLenExp] == '\0');
            UNITTEST_TRUE(!StrT::CmpN<TYPE>(pszOut, sExpected, nLenExp));
            BYTE chEnd2 = (BYTE)pszOut[nLenAlloc - 1];
            UNITTEST_TRUE(chEnd2 == cHeap::kFillUnusedStack);
        }
    }
}

template <typename TYPE>
void GRAYCALL UnitTestFormat(typename StrFormat<TYPE>::STRFORMAT_t pFormat) {  // static
    // http://www.cplusplus.com/reference/cstdio/printf/
    // pFormat = StrT::sprintfN<TYPE>

    UNITTEST_TRUE(pFormat != nullptr);

    TYPE szTmp[StrT::k_LEN_Default];
    UnitTestFormatX(pFormat, TOSPAN(szTmp), _countof(szTmp));
    UnitTestFormatX(pFormat, ToSpan(szTmp, 3), _countof(szTmp));  // Test buffer size overflows/truncation.

#if 0
    UnitTestFormatX(pFormat, cBlob::k_Empty);                     // Test size estimation.

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
