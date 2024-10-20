//! @file cFile.Tests.cpp
// clang-format off
#include "pch.h"
// clang-format on
#include <GrayCore/include/cFile.h>
#include <GrayCore/include/cMime.h>

namespace Gray {
struct UNITTEST_N(cFile) : public cUnitTest {
    void TestWrite(cStringF sFilePath) {
        //
        cFile testfile1;
        HRESULT hRes = testfile1.OpenX(sFilePath, OF_CREATE | OF_WRITE | OF_BINARY);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        cStreamTester::TestWrites(testfile1);
    }

    void TestRead(cStringF sFilePath, bool isString) {
        cFile testfile2;
        HRESULT hRes = testfile2.OpenX(sFilePath, isString ? (OF_READ | OF_TEXT) : (OF_READ | OF_BINARY));
        UNITTEST_TRUE(SUCCEEDED(hRes));

        cFileStatus filestatus1;
        hRes = testfile2.GetFileStatus(filestatus1);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        cStreamTester::TestReads(testfile2, isString);
    }

    void TestGrow(cStringF sFilePath) {
        // open and Grow a file.

        cFile testfile2;
        HRESULT hRes = testfile2.OpenX(sFilePath, OF_READ | OF_WRITE | OF_BINARY);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        cFileStatus filestatus1;
        hRes = testfile2.GetFileStatus(filestatus1);
        UNITTEST_TRUE(SUCCEEDED(hRes));

        // grow
        auto lengthFile = filestatus1.GetFileLength();  // 84
        hRes = testfile2.SetLength(CastN(STREAM_POS_t, lengthFile + 1024));
        UNITTEST_TRUE(SUCCEEDED(hRes));

        testfile2.Close();

        cFileStatus filestatus2;
        hRes = filestatus2.ReadFileStatus(sFilePath);
        UNITTEST_TRUE(SUCCEEDED(hRes));
        UNITTEST_TRUE(filestatus2.GetFileLength() == lengthFile + 1024);
    }

    UNITTEST_METHOD(cFile) {
        //! Create a test file.
        cUnitTests& uts = cUnitTests::I();

        cStringF sFilePath = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("CoreUnitTestFile") _FN(MIME_EXT_txt));
        TestWrite(sFilePath);

        // Read it back.
        TestRead(sFilePath, false);
        TestRead(sFilePath, true);

        TestGrow(sFilePath);

        // Should Fail to delete directory.
        HRESULT hRes = cFile::DeletePath(uts.get_TestOutDir());
        UNITTEST_TRUE(hRes == E_ACCESSDENIED);  // this should fail! E_ACCESSDENIED=WIN32

        hRes = cFile::DeletePath(sFilePath);
        UNITTEST_TRUE(SUCCEEDED(hRes));
    }
};
UNITTEST2_REGISTER(cFile, UNITTEST_LEVEL_t::_Core);
}  // namespace Gray
