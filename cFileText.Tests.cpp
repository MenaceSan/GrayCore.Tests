//
//! @file cFileText.Tests.cpp
//
#include "pch.h"
#include "cFileText.h"
#include "cMime.h"

#if USE_CRT

namespace Gray
{
	UNITTEST2_CLASS(cFileText)
	{
		UNITTEST2_METHOD(cFileText)
		{
			//! @todo test reading cFileText and seek position inside it. Must not be fooled by \r\n and \n.
			cUnitTests& uts = cUnitTests::I();

			cTextPos fp1(cTextPos::k_Invalid);
			fp1 = cTextPos::k_Zero;

			cStringF sFilePathB = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("CFileTextB") _FN(MIME_EXT_txt));
			cFile testfileB;
			HRESULT hRes = testfileB.OpenX(sFilePathB, OF_CREATE | OF_WRITE | OF_BINARY);
			UNITTEST2_TRUE(SUCCEEDED(hRes));

			hRes = testfileB.WriteString(cUnitTests::k_sTextBlob.get_CPtr());
			//UNITTEST2_TRUE(hRes == cUnitTests::k_TEXTBLOB_LEN);
			UNITTEST2_TRUE(SUCCEEDED(hRes));

			cStringF sFilePathT = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("CFileTextT") _FN(MIME_EXT_txt));

			cFileText testfileT;
			hRes = testfileT.OpenX(sFilePathT, OF_CREATE | OF_WRITE | OF_BINARY);
			UNITTEST2_TRUE(SUCCEEDED(hRes));

			hRes = testfileT.WriteString(cUnitTests::k_sTextBlob.get_CPtr());
			UNITTEST2_TRUE(SUCCEEDED(hRes));
			//UNITTEST2_TRUE(hRes == cUnitTests::k_TEXTBLOB_LEN);

		}
	};
	UNITTEST2_REGISTER(cFileText, UNITTEST_LEVEL_Core);
}

#endif // USE_CRT
