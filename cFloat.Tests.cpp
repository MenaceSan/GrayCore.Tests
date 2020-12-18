// 
//! @file cFloat.Tests.cpp
//
#include "pch.h"
#include "cFloat.h"
#include "cArray.h"
#include "cTypes.h"

namespace Gray
{
	UNITTEST2_CLASS(cFloat)
	{
		UNITTEST2_METHOD(cFloat)
		{
			// Test basic assumptions about floats.
			cFloat32 f1(65536);
			cFloat32 f2;
			f2.m_v.u_f = 65536;
			UNITTEST2_TRUE(f1.m_v.u_f == f2.m_v.u_f);

			f1.put_Bits(cFloat32::k_uOne);
			UNITTEST2_TRUE(1.0f == f1.m_v.u_f);
			f2.put_Bits(cFloat32::k_uTwo);
			UNITTEST2_TRUE(2.0f == f2.m_v.u_f);

			cFloat64 d1(65536);
			cFloat64 d2;
			d2.m_v.u_d = 65536;
			UNITTEST2_TRUE(d1.m_v.u_d == d2.m_v.u_d);

			cFloat64 dx1(cTypeLimit<double>::k_Max);
			UNITTEST2_TRUE(dx1.m_v.u_d > 10);
			cFloat64 dx2(cTypeLimit<double>::k_Min);
			UNITTEST2_TRUE(dx2.m_v.u_d < -10);
		}
	};
	UNITTEST2_REGISTER(cFloat, UNITTEST_LEVEL_Core);
}
