//
//! @file StrFormat.Tests.cpp
//
#include "pch.h"
#include "StrFormat.h"
#include "cTypes.h"

namespace Gray
{
	enum STRFORMATTEST_TYPE
	{
		//! @struct Gray::STRFORMATTEST_TYPE
		//! Argument types for testing printf()

		STRFORMATTEST_0,		// no args intentionally.
		STRFORMATTEST_null,

		STRFORMATTEST_s1,		// "abc"
		STRFORMATTEST_s2,		// "abcdef"
		STRFORMATTEST_sA,		// "A string"

		STRFORMATTEST_i0,		// 0
		STRFORMATTEST_i1,		// 123123
		STRFORMATTEST_i2,		// 
		STRFORMATTEST_i5,		// 
		STRFORMATTEST_im2,		// 
		STRFORMATTEST_in,		// 

		STRFORMATTEST_x1,		// 
		STRFORMATTEST_xlg,		// 

		STRFORMATTEST_c1,		// 
		STRFORMATTEST_c2,		// 

		STRFORMATTEST_f0,		// 0.0
		STRFORMATTEST_fp1,		// 
		STRFORMATTEST_f2,		// 
		STRFORMATTEST_f3,		// 3.1416, 3.1416, 3.1416
		STRFORMATTEST_f100,

		STRFORMATTEST_fn,		// -123.456
		STRFORMATTEST_fnz,		// -1.797693e-100 < epsilon?
		STRFORMATTEST_flg,		// 1.797693e+100
		STRFORMATTEST_fmax,

		STRFORMATTEST_QTY,
	};

	struct StrFormatTest
	{
		//! @struct Gray::StrFormatTest
		//! Test a format string.
		STRFORMATTEST_TYPE m_eArgs;
		const char* m_pszFormat;
		const char* m_pszExpected;
	};

	template< typename TYPE>
	StrLen_t UnitTestFormat1(typename StrFormat<TYPE>::STRFORMAT_t pFormat, TYPE* pszOut, StrLen_t nLenOut, const TYPE* pszFormat, int eArgs) // static 
	{
		// Supply test arguments.
		switch (eArgs)
		{
		case STRFORMATTEST_0:
			return (*pFormat)(pszOut, nLenOut, pszFormat);
		case STRFORMATTEST_null:
			return (*pFormat)(pszOut, nLenOut, pszFormat, nullptr);

		case STRFORMATTEST_s1:
			return (*pFormat)(pszOut, nLenOut, pszFormat, StrArg<TYPE>("abc"));
		case STRFORMATTEST_s2:
			return (*pFormat)(pszOut, nLenOut, pszFormat, StrArg<TYPE>("abcdef"));
		case STRFORMATTEST_sA:
			return (*pFormat)(pszOut, nLenOut, pszFormat, StrArg<TYPE>("A string"));

		case STRFORMATTEST_i0:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 0);
		case STRFORMATTEST_i1:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 123123);
		case STRFORMATTEST_i2:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 5, 10);	// m_bWidthArg
		case STRFORMATTEST_i5:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 100, 100, 100, 100, 100);

		case STRFORMATTEST_im2:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 1977, 650000L);

		case STRFORMATTEST_in:
			return (*pFormat)(pszOut, nLenOut, pszFormat, -200, 5);

		case STRFORMATTEST_x1:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 0xA3);
		case STRFORMATTEST_xlg:
			return (*pFormat)(pszOut, nLenOut, pszFormat, (INT64)0x123ABCL);

		case STRFORMATTEST_c1:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 'z');
		case STRFORMATTEST_c2:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 'a', 65);

		case STRFORMATTEST_f0:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 0.0);
		case STRFORMATTEST_fp1:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 0.123);
		case STRFORMATTEST_f2:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 1.123456789012345);
		case STRFORMATTEST_f3:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 3.1416, 3.1416, 3.1416);
		case STRFORMATTEST_f100:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 100.0);

		case STRFORMATTEST_fn:
			return (*pFormat)(pszOut, nLenOut, pszFormat, -123.456);
		case STRFORMATTEST_fnz:
			return (*pFormat)(pszOut, nLenOut, pszFormat, -1.797693e-100);
		case STRFORMATTEST_flg:
			return (*pFormat)(pszOut, nLenOut, pszFormat, 1.797693e+100);
		case STRFORMATTEST_fmax:
			return (*pFormat)(pszOut, nLenOut, pszFormat, cTypeLimit<double>::k_Max);

			// NaN, Inf ?

			//case STRFORMATTEST_b1:
			//	return (*pFormat)(pszOut, nLenOut, pszFormat, 0x550F);
		default:
			break;
		}

		ASSERT(0);
		return 0;
	}

	template< typename TYPE>
	void GRAYCALL UnitTestFormatX(typename StrFormat<TYPE>::STRFORMAT_t pFormat, TYPE* pszOut, StrLen_t nLenOut) // static 
	{
		//! @note ASSUME sprintf() will '\0' terminate the far end even if it isn't needed? (or overflows)
		//! @arg nLenOut = includes room for '\0'
		//! https://www.cs.fsu.edu/~myers/c++/notes/c_io.html

		static const StrFormatTest k_UnitTestData[] =
		{
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

		for (size_t i = 0; i < _countof(k_UnitTestData); i++)
		{
			const StrFormatTest& test = k_UnitTestData[i];

			cStringT<TYPE> sFormat = test.m_pszFormat;
			cStringT<TYPE> sExpected = test.m_pszExpected;

			StrLen_t nLenX = sExpected.GetLength();
			if (nLenX >= nLenOut)
				nLenX = nLenOut - 1;
			UNITTEST2_TRUE(nLenX < StrT::k_LEN_MAX - 1);

			if (pszOut != nullptr)
			{
				cValArray::FillSize<BYTE>(pszOut, StrT::k_LEN_MAX * sizeof(TYPE), cHeap::FILL_UnusedStack);	// fill background.
			}

			StrLen_t nLenRet = UnitTestFormat1<TYPE>(pFormat, pszOut, nLenOut, sFormat, test.m_eArgs);
			if (nLenRet == -1)
			{
				// M$ version will return this if the nLenOut size is too small.
				UNITTEST2_TRUE(2 == nLenX);
				nLenRet = nLenX;
			}
			else
			{
				UNITTEST2_TRUE(nLenRet == nLenX);
			}

			if (pszOut != nullptr)
			{
				// NOTE: M$ vsnprintf() will fill unused data in _DEBUG mode. with 0xFE
				UNITTEST2_TRUE(pszOut[nLenX] == '\0');
				UNITTEST2_TRUE(!StrT::CmpN<TYPE>(pszOut, sExpected, nLenX));
				BYTE chEnd2 = (BYTE)pszOut[StrT::k_LEN_MAX - 1];
				UNITTEST2_TRUE(chEnd2 == cHeap::FILL_UnusedStack);
			}
		}
	}

	template< typename TYPE>
	void GRAYCALL UnitTestFormat(typename StrFormat<TYPE>::STRFORMAT_t pFormat) // static 
	{
		// http://www.cplusplus.com/reference/cstdio/printf/
		// pFormat = StrT::sprintfN<TYPE>

		UNITTEST2_TRUE(pFormat != nullptr);

		TYPE szTmp[StrT::k_LEN_MAX];
		UnitTestFormatX(pFormat, szTmp, STRMAX(szTmp));
		UnitTestFormatX(pFormat, szTmp, 3);		// Test buffer size overflows.

#if 0
		UnitTestFormatX(pFormat, nullptr, 0);	// Test size estimation.

		//*****************************

		// Support C# style string {0} ?
		// nLenRet = (*pFormat)(szTmp, STRMAX(szTmp), CSTRCONST("test{0}test"), StrArg<TYPE>("fedcba"));
		// UNITTEST2_TRUE(nLenRet == 11);	

		// bad pointer. (throw exception?)
		nLenRet = (*pFormat)(szTmp, STRMAX(szTmp), CSTRCONST("test%stest"), 12312323);
		UNITTEST2_TRUE(nLenRet == 11);
#endif
	}

	UNITTEST2_CLASS(StrFormat)
	{
		UNITTEST2_METHOD(StrFormat)
		{
			cTimePerf timeStart1(true);
			UnitTestFormat<wchar_t>(StrFormat<wchar_t>::FormatF);
			UnitTestFormat<char>(StrFormat<char>::FormatF);
			cTimePerf timeStart2(true);
			UnitTestFormat<wchar_t>(StrT::sprintfN<wchar_t>);
			UnitTestFormat<char>(StrT::sprintfN<char>);
			cTimePerf timeStart3(true);
			TIMEPERF_t iDff1 = timeStart1.GetAgeDiff(timeStart2);
			TIMEPERF_t iDff2 = timeStart2.GetAgeDiff(timeStart3);
			UNITTEST2_TRUE(iDff1 > 0);
			UNITTEST2_TRUE(iDff2 > 0);
			// UNITTEST2_TRUE(iDff1 <= iDff2);	// Assume my version is faster.
		}
	};
	UNITTEST2_REGISTER(StrFormat, UNITTEST_LEVEL_Core);
}
