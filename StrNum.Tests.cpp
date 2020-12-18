//
//! @file StrNum.Tests.cpp
//
#include "pch.h"
#include "StrNum.h"
#include "cTypes.h"

namespace Gray
{
	UNITTEST2_CLASS(StrNum)
	{
		void TestDoubleStr(double dVal1, const char* pszVal)
		{
			// Test MUST be reversible. AND do same string to double convert as C++ compiler.
			// Test 1 - Simple reversal of a string.
			double dVal2 = StrNum::toDouble(pszVal, nullptr);
			UNITTEST2_TRUE(dVal2 == dVal1);	// NOT 123.12299999

			char szTmp[StrNum::k_LEN_MAX_DIGITS];
			StrLen_t nLenRet = StrNum::DtoAG2(dVal2, szTmp);	// not clipped decimal.
			UNITTEST2_TRUE(nLenRet > 0);
			UNITTEST2_TRUE(!StrT::Cmp(szTmp, pszVal));
		}

		template <typename _TYPE>
		void TestArray()
		{
			_TYPE aVals[4];
			size_t nRet = StrNum::ToValArray<_TYPE>(aVals, _countof(aVals), "1,2,3,4");
			UNITTEST2_TRUE(nRet == 4);

			char szOut[256];
			StrLen_t nLen = StrNum::ValArrayToA<_TYPE>(szOut, STRMAX(szOut), aVals, _countof(aVals)); // static
			UNITTEST2_TRUE(nLen == 7);
		}

		static StrLen_t GRAYCALL DToATestLegacy(double dVal, OUT char* pszOut, StrLen_t iStrMax, StrLen_t iDecPlaces)
		{
			// Used for compatibility testing.
			char szFormat[64];
			szFormat[0] = '%';
			StrLen_t iLenFormat = 1;
			if (iDecPlaces >= 0)
			{
				szFormat[iLenFormat++] = '.';
				iLenFormat += StrT::UtoA(iDecPlaces, szFormat + iLenFormat, STRMAX(szFormat) - iLenFormat, 10);
			}
			szFormat[iLenFormat++] = 'f';	// will not produce E notation.
			szFormat[iLenFormat] = '\0';

			StrLen_t iLen = StrT::sprintfN<char>(pszOut, iStrMax, szFormat, dVal); // %.2f
																				   // Trim zeros after the .
			if (StrT::HasChar<char>(pszOut, '.'))
			{
				iLen = StrNum::GetTrimCharsLen(pszOut, iLen, '0');
				if (iLen > 0 && pszOut[iLen - 1] == '.') // get rid of the dot too.
				{
					iLen--;
				}
				pszOut[iLen] = '\0';	// eat it.
				if (iLen == 1 && pszOut[0] == '0' && dVal != 0)	// similar effect to %g
				{
					iLen = StrT::sprintfN<char>(pszOut, iStrMax, CSTRCONST("%e"), dVal);
					// Trim 0 before e
					StrLen_t iLenE = StrT::FindCharN<char>(pszOut, 'e');
					if (iLenE > 0)
					{
						StrLen_t iLenE2 = StrNum::GetTrimCharsLen(pszOut, iLenE, '0');
						if (iLenE > iLenE2)
						{
							// e.g. "6.667000e-20"
							cMem::CopyOverlap(pszOut + iLenE2, pszOut + iLenE, ((iLen + 1) - iLenE) * sizeof(char));
						}
					}
				}
			}
			return iLen;
		}

		UNITTEST2_METHOD(StrNum)
		{
			// Test float,double.  

			// Test 1 - Simple reversal.
			TestDoubleStr(0, "0");
			TestDoubleStr(123.123, "123.123");
			TestDoubleStr(-1e-14, "-1e-14");
			// TestDoubleStr(6.667e-20, "6.667e-20");

			// Test 2 - Clipping

			char szTmp[StrNum::k_LEN_MAX_DIGITS];
			StrLen_t nLenRet = StrNum::DtoAG2(123.123, szTmp, 2, '\0');	// Test clip decimal places.
			UNITTEST2_TRUE(!StrT::Cmp(szTmp, "123.12"));

			nLenRet = StrNum::DtoAG2(99.99999, szTmp, 2, '\0');	// Test clip decimal places.
			UNITTEST2_TRUE(!StrT::Cmp(szTmp, "100.00"));

			// Test 3 - Exponent.
			double dVal = StrNum::toDouble("3.141600E+000", nullptr); // dVal = 3.1415999999999999
			dVal -= 3.1415;
			UNITTEST2_TRUE(dVal <= 0.0001);	// near

			nLenRet = StrT::DtoA<char>(dVal, szTmp, STRMAX(szTmp));
			// UNITTEST2_TRUE(!StrT::Cmp(szTmp, "123.123"));

			// Test formats.

			dVal = StrNum::toDouble(".123", nullptr);
			dVal -= 0.123;
			UNITTEST2_TRUE(dVal <= 0.0001);

			dVal = StrNum::toDouble("123.1234567", nullptr);
			UNITTEST2_TRUE(dVal == 123.1234567);

			nLenRet = StrNum::DtoAG2(dVal, szTmp, 6, '\0');	// clip.
			UNITTEST2_TRUE(!StrT::Cmp<char>(szTmp, "123.123457"));	// rounds last digit up.
			nLenRet = StrNum::DtoAG2(dVal, szTmp, 7, '\0');	// clip does nothing.
			UNITTEST2_TRUE(!StrT::Cmp<char>(szTmp, "123.1234567"));	// NO round last digit.

			// test extremes.
			dVal = -0.00000000000001;
			nLenRet = StrNum::DtoAG2(dVal, szTmp);	// "-0" ?
			UNITTEST2_TRUE(!StrT::Cmp<char>(szTmp, "-1e-14"));
			UNITTEST2_TRUE(nLenRet == 6);
			dVal = StrNum::toDouble(szTmp);
			UNITTEST2_TRUE(dVal == -0.00000000000001);

			// DBL_MAX

			// way too much precision.
			dVal = StrNum::toDouble("123.1234567890123456789012345678901234567890123456789012345678901234567890123456789", nullptr);
			UNITTEST2_TRUE(dVal == 123.12345678901235);

			dVal = 123.1234567890123456789012345678901234567890123456789012345678901234567890123456789;
			UNITTEST2_TRUE(dVal == 123.12345678901235);

			// Test Large Exponent.
			dVal = cTypeLimit<double>::k_Max;
			nLenRet = StrNum::DtoAG2(dVal, szTmp);
			UNITTEST2_TRUE(!StrT::Cmp<char>(szTmp, "1.7976931348623157e+308"));

			dVal = cTypeLimit<double>::k_Min;
			nLenRet = StrNum::DtoAG2(dVal, szTmp);
			UNITTEST2_TRUE(!StrT::Cmp<char>(szTmp, "-1.7976931348623157e+308"));

			// Test Exponent sign,
			dVal = StrNum::toDouble("-1.797693e-100");
			UNITTEST2_TRUE(dVal > -1 && dVal < 0);
			nLenRet = StrNum::DtoAG2(dVal, szTmp);
			UNITTEST2_TRUE(!StrT::Cmp<char>(szTmp, "-1.7976929999999997e-100"));	// ???

			// Test convert to float from double.
			float f1 = 1.5f;	// same accuracy
			dVal = 1.5;
			float f2 = (float)dVal;
			UNITTEST2_TRUE(f2 == f1);
			UNITTEST2_TRUE(dVal == f1);

			dVal = 1.23;			// more accurate
			f1 = 1.23f;
			f2 = (float)dVal;
			UNITTEST2_TRUE(f2 == f1);
			UNITTEST2_TRUE(dVal != f1);

			dVal = 1.38;
			f1 = 1.38f;		// more accurate
			f2 = (float)dVal;
			UNITTEST2_TRUE(f2 == f1);
			UNITTEST2_TRUE(dVal != f1);

			// 
			TestArray<double>();
			TestArray<int>();
		}
	};
	UNITTEST2_REGISTER(StrNum, UNITTEST_LEVEL_Core);
}
