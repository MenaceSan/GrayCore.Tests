//! @file cUInt64.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cUInt64.h>

namespace Gray {
struct UNITTEST_N(cUInt64) : public cUnitTest {
    void UnitTestStr(const cUInt64& d1, RADIX_t r) {
        auto sTmp1 = d1.GetStr(r);

        cUInt64 d2(sTmp1, r);
        UNITTEST_TRUE(d1 == d2);
        
        auto sTmp2 = d2.GetStr(r);
        UNITTEST_TRUE(!StrT::Cmp<GChar_t>(sTmp2, sTmp1));

        cUInt64 d3;
        d3.SetStr(sTmp2.get_SpanStr(), r);
        UNITTEST_TRUE(d1 == d3);
        UNITTEST_TRUE(d1 == d2);
    }
    void UnitTestStr(RADIX_t r) {
        cUInt64 d1;
        d1.SetRandomBits(64);
        UnitTestStr(d1, r);
    }

    void UnitTestPrimes() {
        cUnitTests& uts = cUnitTests::I();

        for (cUInt64::UNIT_t i = 2; i < 200; i++) {
            cUInt64 n(i);
            if (n.isPrime()) {
                uts._pLog->addDebugInfoF("Prime=%d", i);
            }
        }
    }

    UNITTEST_METHOD(cUInt64) {
        //! Hold numbers. simple assignment
        const cUInt64 nu1(1);
        const cUInt64 nu2(2);
        const cUInt64 nu19(19);
        const cUInt64 nu25(25);

        UNITTEST_TRUE(nu19 == 19);

        cUInt64 nux1(1234567890);
        UNITTEST_TRUE(nux1.get_Val<UINT32>() == 1234567890);
        cUInt64 nux2(nux1);
        UNITTEST_TRUE(nux1 == nux2);
        UNITTEST_TRUE(nux2.get_Val<UINT32>() == 1234567890);
        nux2 = nu25;
        UNITTEST_TRUE(nux2 == nu25);
        UNITTEST_TRUE(nux2 == 25);
        cUInt64 nux3;

        UnitTestPrimes();

        // strings to numbers
        static const char k_TmpX[] = "1234567890123456789";  // as big as 64 bits will hold.
        static const char k_Tmp16[] = "10000000000000000";   // 16 power.

        bool bRet = nux1.SetStr(TOSPAN_LIT(k_TmpX));
        UNITTEST_TRUE(bRet);

        auto sTmp = nux1.GetStr();
        UNITTEST_TRUE(sTmp.GetLength() > 1);
        UNITTEST_TRUE(TOSPAN_LIT(k_TmpX).IsEqualData(sTmp));

        UnitTestStr(2);
        UnitTestStr(10);
        UnitTestStr(16);
        UnitTestStr(26);

        // Simple Tests
        UNITTEST_TRUE(nu25.isOdd());
        nux1.SetStr(TOSPAN_LIT(k_Tmp16), 0x10);  // hex.

        UNITTEST_TRUE(cUInt64((cUInt64::UNIT_t)0).get_Highest1Bit() == 0);
        UNITTEST_TRUE(cUInt64(1).get_Highest1Bit() == 1);
        UNITTEST_TRUE(cUInt64(4095).get_Highest1Bit() == 12);
        UNITTEST_TRUE(cUInt64(4096).get_Highest1Bit() == 13);

        // Primitives.
        UINT32 u32 = nu19.get_Val<UINT32>();
        UNITTEST_TRUE(u32 == 19);
        UINT64 u64 = nu25.get_Val<UINT64>();
        UNITTEST_TRUE(u64 == 25);

#if 0  // TODO someday Make all these tests work for CUint64
			static const char k_Tmp9s[] = "999999999999999999";	// +1 to get k_Tmp18
			static const char k_Tmp18[] = "1000000000000000000";	// 18 power
			static const char k_Tmp9[] = "1000000000";	// 9 power

			// Bit ops.
			// 25 is binary 11001.
			nux1 = 25;

			iBit = 33;
			nux1 <<= iBit;
			UNITTEST_TRUE(nux1.IsSet(iBit + 4)); // 1
			UNITTEST_TRUE(nux1.IsSet(iBit + 3)); // 1
			UNITTEST_TRUE(!nux1.IsSet(iBit + 2)); // 0
			UNITTEST_TRUE(!nux1.IsSet(iBit + 1)); // 0
			UNITTEST_TRUE(nux1.IsSet(iBit + 0)); // 1
			UNITTEST_TRUE(nux1.get_Highest1Bit() == iBit + 5); // 5

			// Effectively add 2^32.
			nux1.ModBit(iBit + 32, true);
			UNITTEST_TRUE(nux1.IsSet(iBit + 32)); // 1
			nux1 >>= iBit;

			UNITTEST_TRUE(nux1 == cUInt64("4294967321")); // 4294967321
			nux1.ModBit(31, true);
			nux1.ModBit(32, false);
			UNITTEST_TRUE(nux1.get_Val<UINT>() == 2147483673U); // 2147483673

			// NOTE: Do a series of random opposite operations that cancel each other out.
			// bits set/clear, shift up/dn, add/sub, mul/div

			// Bit shifts.
			nux1 = k_TmpX;
			nux2 = nux1;
			nux2 <<= 31;
			// iLen = nux2.GetStr( TOSPAN(szTmp));

			nux3 = nux1;
			nux3 *= cUInt64(1UL << 31);
			// iLen = nux3.GetStr( TOSPAN(szTmp));

			UNITTEST_TRUE(nux2 == nux3);
			nux2 >>= 31;
			UNITTEST_TRUE(nux1 == nux2);

			nux2 <<= 31;
			nux2 <<= 41;
			nux2 <<= 51;
			nux2 <<= 61;
			nux2 >>= 31;
			nux2 >>= 41;
			nux2 >>= 51;
			nux2 >>= 61;
			UNITTEST_TRUE(nux1 == nux2);

			// Add
			nux1 = nu1 + nu2;
			UNITTEST_TRUE(nux1 == 3);
			nux1.SetStr(k_Tmp9s);
			nux1.OpAdd1(1);
			UNITTEST_TRUE(!nux1.isOdd());
			UNITTEST_TRUE(nux1 == cUInt64(k_Tmp32));

			nux2 = k_Tmp16;
			nux3 = nux1 + nux2;
			nux3 += nux1;
			iLen = nux3.GetStr(TOSPAN(szTmp));
			UNITTEST_TRUE(nux3 == cUInt64("200000000000000010000000000000000"));

			// Subtract
			nux1.SetStr(k_Tmp32);
			nux1.OpSubtract1(1);
			UNITTEST_TRUE(nux1.isOdd());
			UNITTEST_TRUE(nux1 == cUInt64(k_Tmp9s));

			nux2 = k_Tmp16;
			nux1++;
			nux3 = nux1 - nux2;
			nux3 -= nux2;
			UNITTEST_TRUE(nux3 == cUInt64("99999999999999980000000000000000"));

			// Multiply()
			nux1.SetStr(k_Tmp32);
			nux1.OpMultiply(2);
			// iLen = nux1.GetStr( TOSPAN(szTmp));
			UNITTEST_TRUE(nux1 == cUInt64("200000000000000000000000000000000"));

			nux2 = k_Tmp16;
			nux3 = nux2 * nux2;
			UNITTEST_TRUE(nux3 == cUInt64(k_Tmp32));
			nux3 *= nux1;
			// iLen = nux3.GetStr( TOSPAN(szTmp));
			UNITTEST_TRUE(nux3 == cUInt64("20000000000000000000000000000000000000000000000000000000000000000"));

			// Divide.
			nux1.SetStr(k_Tmp32);
			nux1.OpDivide(2);
			UNITTEST_TRUE(nux1 == cUInt64("50000000000000000000000000000000"));

			nux1.SetStr(k_Tmp32);
			nux2.SetStr(k_Tmp16);
			nux3 = nux1 / nux2;
			iLen = nux3.GetStr(TOSPAN(szTmp));
			UNITTEST_TRUE(nux3 == nux2);

			// Modulus.
			nux1.SetStr(k_Tmp32);
			nux2 = nux1;
			nux1++;
			nux3 = nux1 % nux2;
			UNITTEST_TRUE(nux3 == 1);

			// Div/Mod combined.
			nux1.SetStr(k_TmpX);
			nux2.SetStr(k_Tmp16);
			cUInt64 quotient, remainder;
			cUInt64::Divide(nux1, nux2, quotient, remainder);
			nux3 = (quotient * nux2) + remainder;	// test reverse Divide operation.
			UNITTEST_TRUE(nux3 == nux1);

			// power math
			nux3.SetPower(cUInt64(k_Tmp16), nu2);
			UNITTEST_TRUE(nux3 == cUInt64(k_Tmp32));

			UNITTEST_TRUE(nu19.isPrime());
			UNITTEST_TRUE(!nu25.isPrime());
			UNITTEST_TRUE(cUInt64("689572171629632424814677540353").isPrime());

			if (cUnitTestCur::IsTestInteractive())
			{
				// This can take a long time.
				// 2^1024 - 2^960 - 1 + 2^64 * { [2^894 pi] + 129093 }

				log.addDebugInfoF("Calculating big prime");

				cUInt64 p;
				p.SetRandomPrime(128);
				UnitTestStr(p, 10);

				static const char* k_Prime = "7";
				cUInt64 p1024(k_Prime);
				UNITTEST_TRUE(p1024.isPrime());
			}
#endif
    }
};
UNITTEST2_REGISTER(cUInt64, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
