//! @file cQueue.Tests.cpp
#include "pch.h"
#include <GrayCore/include/cQueueChunked.h>
#include <GrayCore/include/cQueueDyn.h>

namespace Gray {
struct UNITTEST_N(cQueue) : public cUnitTest {
    static const StrLen_t kUNITTEST_CHUNK = 10;

    /// get cSpan to write to Q
    cSpan<GChar_t> GetSpanTest(StrLen_t i) {
        auto spanText = ToSpanStr<GChar_t>(cUnitTests::k_sTextBlob);
        i %= spanText.GetSize();  // k_sTextBlob = cUnitTests::k_sTextBlob._Len
        return ToSpan(spanText.get_PtrConst() + i, cValT::Min(kUNITTEST_CHUNK, spanText.GetSize() - i));
    }

    void TestChunked() {
        // Read/Write a bunch of stuff to cQueueChunked
        cQueueChunked<512, GChar_t> qc;
        StrLen_t k = 0;
        for (; k < 10000;) {
            const auto src = GetSpanTest(k);
            qc.WriteSpanQ(src);
            k += src.GetSize();
        }

        StrLen_t j = 0;
        for (;;) {
            const auto src = GetSpanTest(j);
            GChar_t junk[kUNITTEST_CHUNK];
            ITERATE_t nReadQty = qc.ReadSpanQ(ToSpan(junk, src.GetSize()));
            if (nReadQty <= 0) break;
            UNITTEST_TRUE(cMem::IsEqual(src, junk, nReadQty * sizeof(GChar_t)));
            j += nReadQty;
        }

        UNITTEST_TRUE(k == j);
        UNITTEST_TRUE(qc.isEmptyQ());
    }

    void TestStatic() {
        //! Read/Write a bunch of stuff to cQueueStatic
        cQueueStatic<512, GChar_t> qs;
        ITERATE_t k = 0;
        for (;;) {
            const auto src = GetSpanTest(k);
            ITERATE_t nWriteQty2 = qs.WriteSpanQ(src);
            if (nWriteQty2 <= 0) break;
            k += nWriteQty2;
        }
        UNITTEST_TRUE(qs.isFullQ());
        ITERATE_t j = 0;
        for (;;) {
            const auto src = GetSpanTest(j);
            GChar_t junk[kUNITTEST_CHUNK];
            ITERATE_t nReadQty = qs.ReadSpanQ(ToSpan(junk, src.GetSize()));
            if (nReadQty <= 0) break;
            UNITTEST_TRUE(cMem::IsEqual(src, junk, nReadQty * sizeof(GChar_t)));
            j += nReadQty;
        }
        UNITTEST_TRUE(k == j);
        UNITTEST_TRUE(qs.isEmptyQ());
    }

    void TestRead() {
        // Read test cQueueRead
        cQueueRead<GChar_t> qr(ToSpanStr<GChar_t>(cUnitTests::k_sTextBlob));
        for (StrLen_t i = 0; i < cUnitTests::k_sTextBlob._Len; i++) {
            GChar_t szTmp[2];
            qr.ReadSpanQ(TOSPAN(szTmp));
        }
    }

    void TestBytes() {
        // Read/Write test. cQueueBytes
        cQueueBytes qb;
        for (StrLen_t i = 0; i < cUnitTests::k_sTextBlob._Len; i++) {
        }
    }

    UNITTEST_METHOD(cQueue) {
        TestChunked();
        TestBytes();
        TestRead();
        TestStatic();
    }
};
UNITTEST2_REGISTER(cQueue, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
