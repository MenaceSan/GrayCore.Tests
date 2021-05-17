//
//! @file cFileDir.Tests.cpp
//
#include "pch.h"
#include "cFileDir.h"
#include "cFilePath.h"

namespace Gray
{
	UNITTEST_CLASS(cFileDir)
	{
		HRESULT TestDevice(cFileDevice & di, const FILECHAR_t * pszDeviceId, cStringL sNameDisplay)
		{
			UINT uDeviceType = cFileDevice::GetDeviceType(pszDeviceId);
			if (uDeviceType <= 0)	// UNKNOWN.
				return S_OK;

			cUnitTests& uts = cUnitTests::I();
			uts.m_pLog->addDebugInfoF("%s Device Type = %d", LOGSTR(sNameDisplay), uDeviceType);

			HRESULT hRes1 = di.UpdateInfo(pszDeviceId);
			if (FAILED(hRes1))
			{
				if (pszDeviceId != nullptr && hRes1 == HRESULT_WIN32_C(ERROR_NOT_READY))
				{
					// some drives won't be ready (if removable). Thats OK.
					uts.m_pLog->addDebugInfoF("%s Not Ready", LOGSTR(sNameDisplay));
				}
				else
				{
					uts.m_pLog->addDebugInfoF("%s ERR='%s'", LOGERR(hRes1));
					UNITTEST_TRUE(SUCCEEDED(hRes1));
				}
				return hRes1;
			}

			uts.m_pLog->addDebugInfoF("%s Vol='%s','%s',ser=0%sh,clen=%u,case=%d",
				LOGSTR(sNameDisplay),
				LOGSTR(di.m_sVolumeName), LOGSTR(di.m_sTypeName),
				LOGSTR2(di.m_nSerialNumber, 0x10),			// Volume serial number (time stamp of last format) e.g. 0x0ca0e613
				di.m_dwMaximumComponentLength,	// e.g. 255 bytes
				di.m_bCaseSensitive);			// e.g. 0x03e700ff, FILE_CASE_SENSITIVE_SEARCH

			FILE_SIZE_t uFreeDiskSpace = cFileDevice::GetDeviceFreeSpace(pszDeviceId);
			UNITTEST_TRUE(uFreeDiskSpace > 0 || uDeviceType == DRIVE_CDROM || uDeviceType == DRIVE_REMOTE);		// has free space or its a read only device like DVD?
			uts.m_pLog->addDebugInfoF("%s FreeSpace=%s", LOGSTR(sNameDisplay), LOGSTR(cStringT<LOGCHAR_t>::GetSizeK(uFreeDiskSpace, 0, true)));

			return S_OK;
		}

		void TestReadDir()
		{
			cStringF sTestInpDir = get_TestInpDir();

			const int s_nMinFiles = 3; // min number of header files assumed to be in this dir.
			const FILECHAR_t* kPattern = _FN(GRAY_NAMES) _FN("*");

			cFileDir fd;
			HRESULT hRes1 = fd.ReadDir(sTestInpDir, kPattern);
			UNITTEST_TRUE(SUCCEEDED(hRes1));
			UNITTEST_TRUE((int)hRes1 >= s_nMinFiles);

			// low level directory reader. does not save the list.
			cFileFind ff(sTestInpDir);
			HRESULT hRes2 = ff.FindFile(nullptr, kPattern);
			UNITTEST_TRUE(SUCCEEDED(hRes2));

			int i = 0;
			for (; SUCCEEDED(hRes2); i++)
			{
				hRes2 = ff.FindFileNext(false);
			}
			UNITTEST_TRUE(i >= s_nMinFiles);
			UNITTEST_TRUE(i == (int)hRes1);	// same number of files.

			// Get status of a single folder.
			cFileStatus fs;
			hRes2 = fs.ReadFileStatus(cFilePath::RemoveFileDirSep(sTestInpDir));
			UNITTEST_TRUE(hRes2 == S_OK);
			UNITTEST_TRUE(fs.isAttrDir());
		}

		void TestCreateFileInDir(cStringF sTestFile, FILESYS_TYPE eFileSysType)
		{
			HRESULT hRes1 = cFileDir::CreateDirForFileX(sTestFile);
			UNITTEST_TRUE(hRes1 == S_OK || hRes1 == S_FALSE);

			// Create a file and write some data to it.
			{
				cFile fileTest;
				hRes1 = fileTest.OpenX(sTestFile, OF_WRITE | OF_CREATE);
				UNITTEST_TRUE(hRes1 == S_OK);
				hRes1 = fileTest.WriteString(k_sTextBlob.get_CPtr());
				UNITTEST_TRUE(hRes1 == cUnitTestCur::k_TEXTBLOB_LEN);
				fileTest.Close();
			}

			// FAIL to read a file as a directory. It should fail.
			cFileDir filedirFail;
			hRes1 = filedirFail.ReadDir(sTestFile);
			UNITTEST_TRUE(hRes1 == HRESULT_WIN32_C(ERROR_DIRECTORY));	// The directory name is invalid.

			static const GChar_t* k_pszTimeChange = _GT("2008-07-09 13:47:10 Z");	// TIME_FORMAT_TZ
			cTimeInt tf;
			HRESULT hRes = tf.SetTimeStr(k_pszTimeChange);
			UNITTEST_TRUE(hRes == 0x15);
			cString sTimeChange2 = tf.GetTimeFormStr(TIME_FORMAT_TZ, TZ_UTC);
			UNITTEST_TRUE(!sTimeChange2.Compare(k_pszTimeChange));

			cTimeFile tfft = tf.GetAsFileTime();
			hRes1 = cFileStatus::WriteFileTimes(sTestFile, &tfft, &tfft);
			UNITTEST_TRUE(hRes1 == S_OK);

			cFileStatus fs2;
			HRESULT hRes2 = fs2.ReadFileStatus(sTestFile);
			UNITTEST_TRUE(hRes2 == S_OK);
			UNITTEST_TRUE(fs2.IsSameChangeTime(tf));

			// NFS merges create and modified times ? Create Time NOT set.
			if (eFileSysType != FILESYS_NFS)
			{
				cString sTimeChange = cTimeInt(fs2.m_timeCreate).GetTimeFormStr(TIME_FORMAT_TZ, TZ_UTC);
				UNITTEST_TRUE(!sTimeChange.Compare(k_pszTimeChange));
			}
		}

		UNITTEST_METHOD(cFileDir)
		{
			// enumerate devices in the system.

			cUnitTests& uts = cUnitTests::I();

			cFileDevice di;
			TestDevice(di, nullptr, "(default)");

			// GetDriveType()
			cArrayString<FILECHAR_t> aDevices;
			HRESULT hRes1 = cFileDevice::GetSystemDeviceList(aDevices);
			UNITTEST_TRUE(SUCCEEDED(hRes1));

			for (int i = 0; i < aDevices.GetSize(); i++)
			{
				cStringL sNameDisplay = cStringL::GetFormatf("[%d],'%s'", i, LOGSTR(aDevices[i]));
				cFileDevice di2;
				TestDevice(di2, aDevices[i], sNameDisplay);
			}

			// high level directory reader. saves a list.
			TestReadDir();

			// Create a complex path then destroy it.
			cStringF sTestDir = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN("a1"));
			cStringF sTestFile = cFilePath::CombineFilePathX(sTestDir, _FN("2/cc3/d4/testfile.txt"));
			TestCreateFileInDir(sTestFile, di.get_FileSysType());

			// Now delete it all.
			hRes1 = cFileDir::DeleteDirFiles(sTestDir);
			UNITTEST_TRUE(hRes1 == S_FALSE);	// 1 dir.
		}
	};
	UNITTEST2_REGISTER(cFileDir, UNITTEST_LEVEL_Core);
}
