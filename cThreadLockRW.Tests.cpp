//! @file cThreadLockRW.Tests.cpp

#include "pch.h"
#include <GrayCore/include/cArrayRef.h>
#include <GrayCore/include/cRandom.h>
#include <GrayCore/include/cThreadBase.h>
#include <GrayCore/include/cThreadLockRW.h>
#include <GrayCore/include/cThreadLockRef.h>
#include <GrayCore/include/cLogEvent.h>

namespace Gray {

struct cTestThreadLockRW : public cThreadLockRW {
    static const size_t kBufferSize = 128;
    BYTE _Buffer[kBufferSize + sizeof(UINT)];
    cInterlockedInt _ThreadsRunning;
    int _MaxReaders = 0;  // Get max concurrent readers detected. get_ReaderCount()

    UINT GetCheckSum() const {
        UINT checkSum = 0;
        for (size_t i = 0; i < kBufferSize; i++) {
            checkSum += _Buffer[i];
        }
        return checkSum;
    }
    UINT& RefCheckSum() {
        return *PtrCast<UINT>(_Buffer + kBufferSize);
    }

    void DoWriteInt() {
        // Write verifiable data to buffer. MUST not be corrupt!
        g_Rand.GetNoise(cMemSpan(_Buffer, kBufferSize));
        // compute check sum.
        RefCheckSum() = GetCheckSum();
    }
    bool IsReadInt() {
        return RefCheckSum() == GetCheckSum();
    }

    void DoRead() {
        // Lock for Read
        cThreadGuardRead guard(*this);
        // Make sure the read valid is CRC ok
        const int iCurrentReaders = this->get_ReaderCount();
        if (iCurrentReaders > _MaxReaders) _MaxReaders = iCurrentReaders;
        // ASSERT NO current Writers !
        UNITTEST_TRUE(IsReadInt());

        if (g_Rand.GetRandUX(3)) {
            cThreadGuardRead guard2(*this);  // Test re-entrant guard.
            UNITTEST_TRUE(IsReadInt());
        } else {
            // const auto guard2(Lock());   // test "upgrade" on same thread?
        }
    }

    void DoWrite() {
        // Lock for Write
        const auto guard(Lock());
        UNITTEST_TRUE(IsReadInt());
        DoWriteInt();

        if (g_Rand.GetRandUX(3)) {
            const auto guard2(Lock());  // Test re-entrant guard.
            UNITTEST_TRUE(IsReadInt());
        } else {
            cThreadGuardRead guard2(*this);  // test "downgrade" on same thread.
            UNITTEST_TRUE(IsReadInt());
        }
    }
};

struct cTestThreadWorker : public cThreadRef {
    cTestThreadLockRW& _Data;
    cTestThreadWorker(cTestThreadLockRW& data) : _Data(data) {}

    THREAD_EXITCODE_t Run() override {  // virtual
        _Data._ThreadsRunning.IncV();
        while (ThreadTick()) {
            if (g_Rand.GetRandUX(3)) {
                _Data.DoRead();  // Most are readers.
            } else {
                _Data.DoWrite();
            }
        }
        _Data._ThreadsRunning.DecV();
        return THREAD_EXITCODE_OK;
    }
};

struct UNITTEST_N(cThreadLockRW) : public cUnitTest {
    void TestRW() {
        // Test cThreadLockRW
        // TODO TEST Test upgrade feature?
        // TODO TEST cThreadGuardRef

        cTestThreadLockRW testData;
        UNITTEST_TRUE(testData.isIdle());
        testData.DoWriteInt();
        UNITTEST_TRUE(testData.isIdle());
        testData.DoWrite();
        UNITTEST_TRUE(testData.isIdle());
        testData.DoRead();
        UNITTEST_TRUE(testData.isIdle());

        static const int kThreads = 15;

        UNITTEST_TRUE(testData._ThreadsRunning == 0);
        cArrayRef<cThreadRef> aThreads;
        for (int i = 0; i < kThreads; i++) {
            cRefPtr<cTestThreadWorker> pThread = new cTestThreadWorker(testData);
            HRESULT hRes = pThread->CreateThread(pThread);
            UNITTEST_TRUE(SUCCEEDED(hRes));
            aThreads.Add(pThread);
        }

        cThreadId::SleepCurrent(2000);  // Let it run for some time.
        // UNITTEST_TRUE(testData._ThreadsRunning == kThreads);

        for (auto pThread : aThreads) {
            pThread->RequestStopThread();
        }
        while (testData._ThreadsRunning.get_Value()) {
            cThreadId::SleepCurrent(1);
        }
        UNITTEST_TRUE(testData._ThreadsRunning == 0);
        UNITTEST_TRUE(testData.isIdle());
        UNITTEST_TRUE(testData._MaxReaders > 3);  // Must see some concurrency.
    }

    void TestRef() {
        // TODO cThreadLockRef.h cThreadLockableRef

        cArrayRef<cLogEvent> a;
        cThreadLockableRef lock;

    }

    UNITTEST_METHOD(cThreadLockRW) {
        TestRW();
        TestRef();
    }
};
UNITTEST2_REGISTER(cThreadLockRW, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
