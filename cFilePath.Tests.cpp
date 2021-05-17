//
//! @file cFilePath.Tests.cpp
//
#include "pch.h"
#include "cFilePath.h"

namespace Gray
{
	UNITTEST_CLASS(cFilePath)
	{
		UNITTEST_METHOD(cFilePath)
		{
			static const FILECHAR_t* k_UnitTest_FilePaths[] =
			{
				// test some odd file names.
				_FN("sdf-fdfs/./sdf.ext"),
				_FN("./sdf.ext"),
				_FN("/sdf-fdfs/../sdf.ext"),
				_FN("/../sdf.ext"),
				_FN("sdf-fdfs/../sdf.ext"),
				_FN("../abcd/c/d/e/../../ef.ext"),
				_FN("c:/..\\abcd\\ef.ext"),
				_FN("../abcd/ef.ext"),
				_FN("..\\abcd\\ef.ext"),
				_FN("goodname\\goodstuff.ext"),	// intentional bad char \g
				_FN("123123123.ext"),
				_FN("asd/123123123.ext"),
				_FN("sdf-fdfs___---_asd/123123123.ext"),
				_FN("abc.efg.hij.ext"),
				_FN("abc.efg.hij..ext"),
				_FN("c:\\Windows\\System32\\smss.exe"),
				_FN("\\Device\\HarddiskVolume2\\Windows\\System32\\smss.exe"),
				_FN("\\\\dennislap\\samplesx\\stuff.txt"),
				// "com1:
				// "\\.\PhysicalDisk1"
				// "\\?\UNC\" = long UNC name. ignores .. relatives.
				// "%SystemDir%", "%WinDir%", "%ProfileDir%", "%SystemRoot%" like REG_EXPAND_SZ ExpandEnvironmentStrings SHGetSpecialFolderPath() ?
				nullptr,
			};

			cUnitTests& uts = cUnitTests::I();

			for (UINT i = 0; i < _countof(k_UnitTest_FilePaths) - 1; i++)
			{
				const FILECHAR_t* pszTest = k_UnitTest_FilePaths[i];

				// GetFileSymbolicName
				cStringA sSymbolicName(cFilePath::GetFileSymbolicName(pszTest, nullptr));
				uts.m_pLog->addDebugInfoF("File='%s', Symbol='%s'", LOGSTR(pszTest), LOGSTR(sSymbolicName));

				sSymbolicName = cFilePath::GetFileSymbolicName(pszTest, "PREFIX");
				UNITTEST_TRUE(sSymbolicName.GetLength());

				// MakeProperPath
				FILECHAR_t szOut[_MAX_PATH];
				StrLen_t nLen2 = cFilePath::MakeProperPath(szOut, STRMAX(szOut), pszTest);
				StrLen_t nLen1 = StrT::CopyLen(szOut, pszTest, STRMAX(szOut));
				StrLen_t nLen3 = cFilePath::MakeProperPath(szOut);

				UNITTEST_TRUE(nLen1 >= nLen2);
				UNITTEST_TRUE(nLen2 == nLen3);
			}

			FILECHAR_t* pszUpDir2 = cFilePath::GetFilePathUpDir2(_FN("/a/b/c/d/ef.ext"), k_StrLen_UNK, 2);
			UNITTEST_TRUE(!StrT::CmpI(pszUpDir2, _FN("d/ef.ext")));
			pszUpDir2 = cFilePath::GetFilePathUpDir2(_FN("/a/b/c/d/ef.ext"), k_StrLen_UNK, -2);
			UNITTEST_TRUE(!StrT::CmpI(pszUpDir2, _FN("b/c/d/ef.ext")));

			cStringF sUpDir1 = cFilePath::GetFilePathUpDir1(_FN("sdf:/dir1/dir2/dir3/dir4"), k_StrLen_UNK, 1);
			UNITTEST_TRUE(!sUpDir1.Compare(_FN("sdf:/dir1/dir2/dir3")));

			// combine and make proper.
			cStringF strFilePath = cFilePath::CombineFilePathF('/', _FN("a"), _FN("b\\c"), _FN("d/ef.ext"), nullptr);
			UNITTEST_TRUE(!strFilePath.Compare(_FN("a/b/c/d/ef.ext")));
		}
	};
	UNITTEST2_REGISTER(cFilePath, UNITTEST_LEVEL_Core);
}
