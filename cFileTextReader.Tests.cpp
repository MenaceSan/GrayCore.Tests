//
//! @file cFileTextReader.Tests.cpp
//
#include "pch.h"
#include "cFileTextReader.h"
#include "cMime.h"

namespace Gray
{
	UNITTEST2_CLASS(cFileTextReader)
	{
		const FILECHAR_t* k_TestFileIni = _FN("cIniFileUnitTest") _FN(MIME_EXT_ini); // static

		UNITTEST2_METHOD(cFileTextReader)
		{
			//! test reading cFileTextReader.
			//! @note any text changes to this file can invalidate the test results.

			cStringF sFilePath = cFilePath::CombineFilePathX(get_TestInpDir(), k_TestFileIni);

			static const int k_MaxLineLen = 180;	// was cStream::k_FILE_BLOCK_SIZE 256. Assume no other line is this long for my test.

			cFileTextReader tr(k_MaxLineLen);
			HRESULT hRes = tr.OpenX(sFilePath, OF_READ | OF_TEXT | OF_SHARE_DENY_NONE | OF_CACHE_SEQ);
			UNITTEST2_TRUE(SUCCEEDED(hRes));

			int iLineNumber = 1;	// 1 based.
			for (;; )
			{
				const char* pszLine = nullptr;
				hRes = tr.ReadStringLine(&pszLine);
				UNITTEST2_TRUE(SUCCEEDED(hRes));
				if (hRes == 0)
					break;

				//*** Make this over k_MaxLineLen chars long ****************************************************************************************************************************************************

				if (hRes >= k_MaxLineLen)
				{
					// Warning  line length was too long !
					UNITTEST2_TRUE(iLineNumber == 167);	// Fix this if source changes.
					DEBUG_MSG(("line %d length was > %d", iLineNumber, hRes));
				}
				else
				{
					iLineNumber++;	// don't count split lines.
				}

				UNITTEST2_TRUE(hRes <= k_MaxLineLen);
			}

			UNITTEST2_TRUE(iLineNumber == 56);	// Fix this if source changes.
		}
	};
	UNITTEST2_REGISTER(cFileTextReader, UNITTEST_LEVEL_Core);
}

