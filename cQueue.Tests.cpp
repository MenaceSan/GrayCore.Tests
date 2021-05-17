// 
//! @file cQueue.Tests.cpp
//
#include "pch.h"
#include "cQueue.h"
#include "cQueueChunked.h"

namespace Gray
{
	UNITTEST_CLASS(cQueue)
	{
		const GChar_t* UnitTest_GetSrc(ITERATE_t i, ITERATE_t & riSrcLenMax)
		{
			//! k_sTextBlob = cUnitTests::k_TEXTBLOB_LEN
			i %= cUnitTests::k_TEXTBLOB_LEN;
			riSrcLenMax = cUnitTests::k_TEXTBLOB_LEN - i;
			return static_cast<const GChar_t*>(cUnitTests::k_sTextBlob) + i;
		}

		UNITTEST_METHOD(cQueue)
		{
			// Read test cQueueRead
			cQueueRead<GChar_t> qr(k_sTextBlob, 0, cUnitTests::k_TEXTBLOB_LEN);

			for (StrLen_t i = 0; i < cUnitTests::k_TEXTBLOB_LEN; i++)
			{
				GChar_t szTmp[2];
				qr.ReadQty(szTmp, _countof(szTmp)); // STRMAX
			}

			// Read/Write test. cQueueBytes
			cQueueBytes qb;
			for (StrLen_t i = 0; i < cUnitTests::k_TEXTBLOB_LEN; i++)
			{
			}

			const int UNITTEST_CHUNK = 10;

			ITERATE_t iSrcLenMax;

			//! Read/Write a bunch of stuff to cQueueStatic
			cQueueStatic<GChar_t, 512> qs;
			int k = 0;
			for (;;)
			{
				const GChar_t* pSrc = UnitTest_GetSrc(k, iSrcLenMax);
				int iPut = qs.WriteQty(pSrc, MIN(UNITTEST_CHUNK, iSrcLenMax));
				if (iPut <= 0)
					break;
				k += iPut;
			}
			UNITTEST_TRUE(qs.isFullQ());
			ITERATE_t j = 0;
			for (;;)
			{
				const GChar_t* pSrc = UnitTest_GetSrc(j, iSrcLenMax);
				GChar_t junk[UNITTEST_CHUNK];
				ITERATE_t nReadQty = qs.ReadQty(junk, MIN(iSrcLenMax, (int)_countof(junk)));
				if (nReadQty <= 0)
					break;
				UNITTEST_TRUE(cMem::IsEqual(pSrc, junk, nReadQty));
				j += nReadQty;
			}
			UNITTEST_TRUE(k == j);
			UNITTEST_TRUE(qs.isEmptyQ());

			// Read/Write a bunch of stuff to cQueueChunked
			cQueueChunked<GChar_t> qc(512);
			for (k = 0; k < 10000;)
			{
				const GChar_t* pSrc = UnitTest_GetSrc(k, iSrcLenMax);
				int iPut = qc.WriteQty(pSrc, MIN(UNITTEST_CHUNK, iSrcLenMax));
				k += iPut;
			}
			for (j = 0;;)
			{
				const GChar_t* pSrc = UnitTest_GetSrc(j, iSrcLenMax);
				GChar_t junk[UNITTEST_CHUNK];
				ITERATE_t nReadQty = qc.ReadQty(junk, MIN(iSrcLenMax, (int)_countof(junk)));
				if (nReadQty <= 0)
					break;
				UNITTEST_TRUE(cMem::IsEqual(pSrc, junk, nReadQty));
				j += nReadQty;
			}

			UNITTEST_TRUE(k == j);
			UNITTEST_TRUE(qc.isEmptyQ());
		}
	};
	UNITTEST2_REGISTER(cQueue, UNITTEST_LEVEL_Core);
}
