// 
//! @file cIniObject.Tests.cpp
//
#include "pch.h"
#include "cIniObject.h"

namespace Gray
{
	UNITTEST2_CLASS(cIniObject)
	{
		// TODO: UNITTEST cIniObject
		class cUnitTestIniObject : public cIniObject
		{
		public:
			virtual IPROPIDX_t get_PropQty(void) const
			{
				// IIniObjectDef
				return 0;
			}
			virtual const IniChar_t* get_PropName(IPROPIDX_t ePropIdx) const
			{
				// IIniObjectDef
				UNREFERENCED_PARAMETER(ePropIdx);
				return nullptr;
			}
			virtual IPROPIDX_t FindProp(const IniChar_t* pName) const
			{
				// IIniObjectDef
				UNREFERENCED_PARAMETER(pName);
				return -1;
			}
			HRESULT PropSetN(IPROPIDX_t ePropIdx, const IniChar_t* pszName)
			{
				// IIniObjectWriteN
				UNREFERENCED_PARAMETER(ePropIdx);
				UNREFERENCED_PARAMETER(pszName);
				return E_NOTIMPL;
			}
			virtual HRESULT PropEnum(IPROPIDX_t ePropIdx, OUT cStringI& rsValue, cStringI* psKey = nullptr) const
			{
				// IIniBaseEnumerator
				UNREFERENCED_PARAMETER(ePropIdx);
				UNREFERENCED_REFERENCE(rsValue);
				UNREFERENCED_PARAMETER(psKey);
				return E_NOTIMPL;
			}
		};

		UNITTEST2_METHOD(cIniObject)
		{
			cUnitTestIniObject obj;


		}
	};
	UNITTEST2_REGISTER(cIniObject, UNITTEST_LEVEL_Core);
}
