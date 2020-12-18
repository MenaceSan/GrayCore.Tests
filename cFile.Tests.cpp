//
//! @file cFile.Tests.cpp
//
#include "pch.h"
#include "cFile.h"
#include "cMime.h"

namespace Gray
{
	GRAYCORE_TEST_LINK void GRAYCALL UnitTest_Write(cStreamOutput& testfile1) // static
	{
		//! Write strings to it.
		for (ITERATE_t i = 0; !cUnitTests::k_asTextLines[i].isNull(); i++)
		{
			HRESULT hRes = testfile1.WriteString(cUnitTests::k_asTextLines[i].get_CPtr());
			UNITTEST2_TRUE(SUCCEEDED(hRes));
			testfile1.WriteString(_GT(STR_NL));
		}
	}

	GRAYCORE_TEST_LINK void GRAYCALL UnitTest_Read(cStreamInput& stmIn, bool bString) // static
	{
		//! Other side of UnitTest_Write()
		//! Read strings from it (as binary).
		GChar_t szTmp[256];

		for (ITERATE_t j = 0; !cUnitTests::k_asTextLines[j].isNull(); j++)
		{
			const GChar_t* pszLine = cUnitTests::k_asTextLines[j];
			StrLen_t iLenStr = StrT::Len(pszLine);
			UNITTEST2_TRUE(iLenStr < (StrLen_t)STRMAX(szTmp));
			size_t nSizeBytes = (iLenStr + 1) * sizeof(GChar_t);
			HRESULT hResRead = bString ? stmIn.ReadStringLine(szTmp, STRMAX(szTmp)) : stmIn.ReadX(szTmp, nSizeBytes);
			UNITTEST2_TRUE(hResRead == (HRESULT)(bString ? (iLenStr + 1) : nSizeBytes));
			UNITTEST2_TRUE(!cMem::Compare(szTmp, pszLine, iLenStr * sizeof(GChar_t)));	// pszLine has no newline.
			UNITTEST2_TRUE(szTmp[iLenStr] == '\n');
		}

		// Check for proper read past end of file.
		HRESULT hResRead = stmIn.ReadX(szTmp, STRMAX(szTmp));
		UNITTEST2_TRUE(hResRead == 0);
		hResRead = stmIn.ReadX(szTmp, STRMAX(szTmp));
		UNITTEST2_TRUE(hResRead == 0);
	}

	UNITTEST2_CLASS(cFile)
	{
		UNITTEST2_METHOD(cFile)
		{
			//! Create a test file.
			cUnitTests& uts = cUnitTests::I();
			
			HRESULT hRes;
			cStringF sFilePath = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("CoreUnitTestFile") _FN(MIME_EXT_txt));

			{
				cFile testfile1;
				hRes = testfile1.OpenX(sFilePath, OF_CREATE | OF_WRITE | OF_BINARY);
				UNITTEST2_TRUE(SUCCEEDED(hRes));
				UnitTest_Write(testfile1);
			}

			// Read it back.
			for (int i = 0; i < 2; i++)
			{
				cFile testfile2;
				hRes = testfile2.OpenX(sFilePath, OF_READ | OF_BINARY);
				UNITTEST2_TRUE(SUCCEEDED(hRes));
				cFileStatus filestatus2;
				hRes = testfile2.GetFileStatus(filestatus2);
				UNITTEST2_TRUE(SUCCEEDED(hRes));
				UnitTest_Read(testfile2, (bool)i);
			}

			// Should Fail to delete directory.
			hRes = cFile::DeletePath(uts.get_TestOutDir());
			UNITTEST2_TRUE(hRes == E_ACCESSDENIED);	// this should fail! E_ACCESSDENIED=WIN32
		}
	};
	UNITTEST2_REGISTER(cFile, UNITTEST_LEVEL_Core);
}
