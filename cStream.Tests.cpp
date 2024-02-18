//
//! @file cStream.Tests.cpp
//

#include "pch.h"
#include <GrayCore/include/cFile.h>
#include <GrayCore/include/cRandom.h>
#include <GrayCore/include/cStream.h>
#include <GrayCore/include/cStreamQueue.h>
#include <GrayCore/include/cTypes.h>

namespace Gray {
GRAYCORE_TEST_LINK void GRAYCALL UnitTest_StreamIntegrity(cStreamOutput& stmOut, cStreamInput& stmIn, size_t nSizeTotal) {
    // Write to streams in random block sizes and make sure i read the same back.
    // @arg nSizeTotal = How much to write/test total ?
    cUnitTests& uts = cUnitTests::I();

    size_t iSizeBlock = g_Rand.GetRandUX(1024) + 100;  // TODO Make random range bigger !! 2k ?
    cBlob blobWrite(iSizeBlock * 2);
    g_Rand.GetNoise(cMemSpan(blobWrite, iSizeBlock));
    cMem::Copy(blobWrite.get_DataW<BYTE>() + iSizeBlock, blobWrite.get_DataC(), iSizeBlock);  // double it.

    size_t iSizeWriteTotal = 0;
    size_t iSizeReadTotal = 0;

    HRESULT hRes;
    cBlob blobRead(iSizeBlock);
    cTimeSys tStart = cTimeSys::GetTimeNow();
    size_t nSizeReal;

    int i = 0;
    for (;; i++) {
        UNITTEST_TRUE(iSizeReadTotal <= iSizeWriteTotal);

        if (iSizeWriteTotal < nSizeTotal)  // write more?
        {
            size_t iSizeWriteBlock = g_Rand.GetRandUX((UINT)(iSizeBlock - 1)) + 1;
            if (iSizeWriteTotal + iSizeWriteBlock > nSizeTotal) iSizeWriteBlock = nSizeTotal - iSizeWriteTotal;
            UNITTEST_TRUE(iSizeWriteBlock <= iSizeBlock);
            hRes = stmOut.WriteX(blobWrite.get_DataC<BYTE>() + (iSizeWriteTotal % iSizeBlock), iSizeWriteBlock);
            UNITTEST_TRUE(SUCCEEDED(hRes));
            nSizeReal = (size_t)hRes;
            UNITTEST_TRUE(nSizeReal <= iSizeWriteBlock);
            iSizeWriteTotal += nSizeReal;
            UNITTEST_TRUE(iSizeWriteTotal <= nSizeTotal);
        }

        UNITTEST_TRUE(iSizeReadTotal <= iSizeWriteTotal);

        size_t iSizeReadBlock = g_Rand.GetRandUX((UINT)(iSizeBlock - 1)) + 1;
        UNITTEST_TRUE(iSizeReadBlock <= iSizeBlock);
        BYTE* pRead = blobRead.get_DataW();
        hRes = stmIn.ReadX(pRead, iSizeReadBlock);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        nSizeReal = (size_t)hRes;
        UNITTEST_TRUE(nSizeReal <= iSizeReadBlock);

        // Make sure i read correctly.
        const BYTE* pWrite = blobWrite.get_DataC<BYTE>() + (iSizeReadTotal % iSizeBlock);
        bool isEqual = cMem::IsEqual(pWrite, pRead, nSizeReal);
        UNITTEST_TRUE(isEqual);
        iSizeReadTotal += nSizeReal;
        UNITTEST_TRUE(iSizeReadTotal <= iSizeWriteTotal);

        if (iSizeReadTotal >= nSizeTotal)  // done?
            break;
        if (!uts.IsTestInteractive() && tStart.get_AgeSec() > 100) {
            UNITTEST_TRUE(false);
            return;
        }
    }
}

struct UNITTEST_N(cStream) : public cUnitTest {
    void Test_StreamSize(cStream& q, bool isQ) {
        //! Write stuff to a stream and read back. WriteSize()

        UNITTEST_TRUE(q.GetLength() == 0);
        STREAM_POS_t nRetSeek = q.GetPosition();
        UNITTEST_TRUE(nRetSeek == 0);

        const ITERATE_t k_Iterations = 64;  // fill 64 bits by doubling.
        const size_t k_SizeWrite = 86;

        ITERATE_t iIterations = 0;
        size_t iInc = 1;
        for (size_t i = 0; i < cTypeLimit<DWORD>::Max() && iIterations <= k_Iterations; i += iInc, iInc *= 2, iIterations++) {
            HRESULT hRes = q.WriteSize(i);
            UNITTEST_TRUE(SUCCEEDED(hRes) && hRes >= 1);
        }

        UNITTEST_TRUE(q.GetLength() == k_SizeWrite);

        nRetSeek = q.GetPosition();

        if (isQ) {
            UNITTEST_TRUE(nRetSeek == 0);
        } else {
            UNITTEST_TRUE(nRetSeek == k_SizeWrite);
            q.SeekX(0, SEEK_t::_Set);
        }

        ITERATE_t iIterations2 = 0;
        iInc = 1;
        for (size_t i = 0; i < cTypeLimit<DWORD>::Max() && iIterations2 <= k_Iterations; i += iInc, iInc *= 2, iIterations2++) {
            size_t nSizeRead = 0;
            HRESULT hRes = q.ReadSize(OUT nSizeRead);
            UNITTEST_TRUE(SUCCEEDED(hRes));
            UNITTEST_TRUE(i == nSizeRead);
        }

        nRetSeek = q.GetPosition();
        UNITTEST_TRUE(nRetSeek == k_SizeWrite);
        UNITTEST_TRUE(iIterations == iIterations2);

        UNITTEST_TRUE(q.GetLength() == k_SizeWrite);
    }

    UNITTEST_METHOD(cStream) {
        //! ReadSize, WriteSize()

        g_Rand.InitSeedOS();

        cStreamQueue q;
        Test_StreamSize(q, true);
        UNITTEST_TRUE(q.isEmptyQ());  // all read back.
        UNITTEST_TRUE(q.get_ReadQty() == 0);

        UnitTest_StreamIntegrity(q, q, 10000 + g_Rand.GetRandUX(500000));
        UNITTEST_TRUE(q.isEmptyQ());  // all read back.
        UNITTEST_TRUE(q.get_ReadQty() == 0);

        cFile fileTmp;
        HRESULT hRes = fileTmp.OpenCreate(cAppState::I().GetTempFile(), OF_CREATE | OF_READWRITE | OF_BINARY);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        Test_StreamSize(fileTmp, false);
    }
};
UNITTEST2_REGISTER(cStream, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
