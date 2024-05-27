//! @file pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#pragma once

#define USE_UNICODE 0	// Why does UNICODE get flipped on ??
#define USE_UNITTESTS_MS

#ifndef GRAYCORE_TEST_LINK
#if defined(_MFC_VER) || defined(GRAY_STATICLIB)	// GRAY_STATICLIB or _MFC_VER can be defined to make Gray* all static lib
#define GRAYCORE_TEST_LINK  // not in a DLL
#else
#define GRAYCORE_TEST_LINK __DECL_EXPORT
#endif
#endif

// add headers that you want to pre-compile here
#include "cUnitTestMs.h"

#endif // PCH_H
