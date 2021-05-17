// 
//! @file cIniObject.Tests.cpp
//
#include "pch.h"
#include "cIniObject.h"

namespace Gray
{
	UNITTEST_CLASS(cIniObject)
	{
		// TODO: UNITTEST cIniObject
		class cUnitTestIniObject : public cIniObject
		{
		public:
			IPROPIDX_t get_PropQty(void) const override
			{
				// IIniObjectDef
				return 0;
			}
			const IniChar_t* get_PropName(IPROPIDX_t ePropIdx) const override
			{
				// IIniObjectDef
				UNREFERENCED_PARAMETER(ePropIdx);
				return nullptr;
			}
			IPROPIDX_t FindProp(const IniChar_t* pName) const override
			{
				// IIniObjectDef
				UNREFERENCED_PARAMETER(pName);
				return -1;
			}
			HRESULT PropSet(const IniChar_t* pszPropTag, const IniChar_t* pszValue) override
			{
				return E_NOTIMPL;
			}
			HRESULT PropGetEnum(IPROPIDX_t ePropIdx, OUT cStringI& rsValue, cStringI* psKey = nullptr) const override
			{
				// IIniBaseEnumerator
				UNREFERENCED_PARAMETER(ePropIdx);
				UNREFERENCED_REFERENCE(rsValue);
				UNREFERENCED_PARAMETER(psKey);
				return E_NOTIMPL;
			}
		};

		UNITTEST_METHOD(cIniObject)
		{
			cUnitTestIniObject obj;


		}
	};
	UNITTEST2_REGISTER(cIniObject, UNITTEST_LEVEL_Core);
}
