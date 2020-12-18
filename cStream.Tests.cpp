//
//! @file cStream.Tests.cpp
//

#include "pch.h"
#include "cStream.h"
#include "cStreamQueue.h"
#include "cRandom.h"
#include "cTypes.h"

namespace Gray
{
	GRAYCORE_TEST_LINK void GRAYCALL UnitTest_StreamIntegrity(cStreamOutput& stmOut, cStreamInput& stmIn, size_t nSizeTotal)
	{
		// Write to streams in random block sizes and make sure i read the same back.
		// @arg nSizeTotal = How much to write/test total ?
		cUnitTests& uts = cUnitTests::I();

		size_t iSizeBlock = g_Rand.GetRandUX(1024) + 100;	// TODO Make random range bigger !! 2k ?
		cHeapBlock blockwrite(iSizeBlock * 2);
		g_Rand.GetNoise(blockwrite, iSizeBlock);
		::memcpy(blockwrite.get_DataBytes() + iSizeBlock, blockwrite.get_DataBytes(), iSizeBlock);	// double it.

		size_t iSizeWriteTotal = 0;
		size_t iSizeReadTotal = 0;

		HRESULT hRes;
		cHeapBlock blockread(iSizeBlock);
		cTimeSys tStart = cTimeSys::GetTimeNow();
		size_t nSizeReal;

		int i = 0;
		for (;; i++)
		{
			UNITTEST2_TRUE(iSizeReadTotal <= iSizeWriteTotal);

			if (iSizeWriteTotal < nSizeTotal)	// write more?
			{
				size_t iSizeWriteBlock = g_Rand.GetRandUX((UINT)(iSizeBlock - 1)) + 1;
				if (iSizeWriteTotal + iSizeWriteBlock > nSizeTotal)
					iSizeWriteBlock = nSizeTotal - iSizeWriteTotal;
				UNITTEST2_TRUE(iSizeWriteBlock <= iSizeBlock);
				hRes = stmOut.WriteX(blockwrite.get_DataBytes() + (iSizeWriteTotal % iSizeBlock), iSizeWriteBlock);
				UNITTEST2_TRUE(SUCCEEDED(hRes));
				nSizeReal = (size_t)hRes;
				UNITTEST2_TRUE(nSizeReal <= iSizeWriteBlock);
				iSizeWriteTotal += nSizeReal;
				UNITTEST2_TRUE(iSizeWriteTotal <= nSizeTotal);
			}

			UNITTEST2_TRUE(iSizeReadTotal <= iSizeWriteTotal);

			size_t iSizeReadBlock = g_Rand.GetRandUX((UINT)(iSizeBlock - 1)) + 1;
			UNITTEST2_TRUE(iSizeReadBlock <= iSizeBlock);
			BYTE* pRead = blockread.get_DataBytes();
			hRes = stmIn.ReadX(pRead, iSizeReadBlock);
			UNITTEST2_TRUE(SUCCEEDED(hRes));
			nSizeReal = (size_t)hRes;
			UNITTEST2_TRUE(nSizeReal <= iSizeReadBlock);

			// Make sure i read correctly.
			const BYTE* pWrite = blockwrite.get_DataBytes() + (iSizeReadTotal % iSizeBlock);
			COMPARE_t iRet = cMem::Compare(pWrite, pRead, nSizeReal);
			UNITTEST2_TRUE(iRet == 0);
			iSizeReadTotal += nSizeReal;
			UNITTEST2_TRUE(iSizeReadTotal <= iSizeWriteTotal);

			if (iSizeReadTotal >= nSizeTotal)	// done?
				break;
			if (!uts.IsTestInteractive() && tStart.get_AgeSec() > 100)
			{
				UNITTEST2_TRUE(false);
				return;
			}
		}
	}

	UNITTEST2_CLASS(cStream)
	{
	public:
		void UnitTest_StreamSize(cStream & q)
		{
			//! Write size to a stream and read back.

			ITERATE_t iIterations = 0;
			size_t iInc = 1;
			for (size_t i = 0; i < cTypeLimit<DWORD>::k_Max && iIterations <= 64; i += iInc, iInc *= 2, iIterations++)
			{
				HRESULT hRes = q.WriteSize(i);
				UNITTEST2_TRUE(SUCCEEDED(hRes));
			}

			STREAM_SEEKRET_t nRetSeek = q.GetPosition();
			UNITTEST2_TRUE(nRetSeek == 0);
			UNITTEST2_TRUE(q.GetLength() == 86);

			ITERATE_t iIterations2 = 0;
			iInc = 1;
			for (size_t i = 0; i < cTypeLimit<DWORD>::k_Max && iIterations2 <= 64; i += iInc, iInc *= 2, iIterations2++)
			{
				size_t nSizeRead;
				HRESULT hRes = q.ReadSize(nSizeRead);
				UNITTEST2_TRUE(SUCCEEDED(hRes));
				UNITTEST2_TRUE(i == nSizeRead);
			}

			nRetSeek = q.GetPosition();
			UNITTEST2_TRUE(nRetSeek == 86);
			UNITTEST2_TRUE(iIterations == iIterations2);
		}

		UNITTEST2_METHOD(cStream)
		{
			//! ReadSize, WriteSize()
			cStreamQueue q;
			UnitTest_StreamSize(q);
			UNITTEST2_TRUE(q.isEmptyQ());	// all read back.
			UNITTEST2_TRUE(q.get_ReadQty() == 0);

			UnitTest_StreamIntegrity(q, q, 10000 + g_Rand.GetRandUX(500000));
			UNITTEST2_TRUE(q.isEmptyQ());	// all read back.
			UNITTEST2_TRUE(q.get_ReadQty() == 0);
		}
	};

	UNITTEST2_REGISTER(cStream, UNITTEST_LEVEL_Core);

}

