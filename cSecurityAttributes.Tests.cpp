//
//! @file cSecurityAttributes.Tests.cpp
//
#include "pch.h"
#include "cSecurityAttributes.h"
#include "cOSUser.h"
#include "cFile.h"

#if defined(_WIN32) && ! defined(UNDER_CE)

namespace Gray
{
	UNITTEST2_CLASS(cSecurityAttributes)
	{
		UNITTEST2_METHOD(cSecurityAttributes)
		{
			// cSecurityId sidUsers(DOMAIN_ALIAS_RID_USERS);
			// cSecurityId sidAdmins(DOMAIN_ALIAS_RID_ADMINS);
			cUnitTests& uts = cUnitTests::I();

			cSecurityId sidUser;
			cOSUserToken token;
			HRESULT hRes = token.GetSID(sidUser);
			UNITTEST2_TRUE(SUCCEEDED(hRes));
			uts.m_pLog->addInfoF("sidUser='%s'", LOGSTR(sidUser.GetStringSID()));

			cSecurityId sidEveryone(WinLocalSid);
			uts.m_pLog->addInfoF("WinLocalSid='%s'", LOGSTR(sidEveryone.GetStringSID()));

			cSecurityACL dacl1(sidEveryone, GENERIC_ALL);
			cSecurityAttributes sa1(false, dacl1);

			cSecurityAttribsWKS saNull(WinNullSid);	// anyone.
			cSecurityAttribsWKS sa2(WinBuiltinUsersSid);	// WinBuiltinPowerUsersSid

			// TODO: Create a file and assign it this privilege.
			cStringF sPathOut = cFilePath::CombineFilePathX(uts.get_TestOutDir(), _FN(GRAY_NAMES) _FN("cSecurityAttributes") _FN(MIME_EXT_txt));
			hRes = cFile::DeletePath(sPathOut);

			cFile fileTest;
			hRes = fileTest.OpenCreate(sPathOut, OF_WRITE | OF_CREATE, sa1);
			UNITTEST2_TRUE(SUCCEEDED(hRes));
			hRes = fileTest.WriteX("WinLocalSid\n", 5);
			UNITTEST2_TRUE(SUCCEEDED(hRes));

			cSecurityAttributes sa3;


		}
	};
	UNITTEST2_REGISTER(cSecurityAttributes, UNITTEST_LEVEL_Core);
}

#endif
