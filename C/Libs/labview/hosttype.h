#ifndef _hosttype_H
#define _hosttype_H
/**
	NI CONFIDENTIAL
	(c) Copyright 1990-2011 by National Instruments Corp.
	All rights reserved.

	@file
	@brief	Host specific definitions, etc.

*/

#if Mac // use forward includes to define types needed for managerp.h
struct EventRecord;
struct RGBColor;
typedef struct Palette *PalettePtr, **PaletteHandle;
typedef struct Cursor *CursPtr, **CursHandle;
typedef struct Picture  *PicPtr, **PicHandle;
typedef struct GDevice *GDPtr, **GDHandle;
typedef struct OpaqueWindowPtr *WindowPtr;
typedef struct OpaqueMenuRef *MenuRef,  *MenuHandle;
typedef struct OpaqueGrafPtr *GrafPtr, *CGrafPtr, *GWorldPtr;
struct BitMap;
struct Pattern;
#include <stdio.h>

#endif	/* Mac */

#if Unix
#include <stdio.h>
#include <string.h>
#endif /* Unix */

#if MSWin
#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif
#if _MSC_VER <= 1499 // MSVC 8.0 or earlier
#define _WIN32_WINNT 0x0400
#else
#if (OpSystem==kMSWin32)
#define _WIN32_WINNT 0x0500
#else
#define _WIN32_WINNT 0x0600
#endif
#endif
#pragma warning (push)
#pragma warning (disable : 4701) /* local variable 'XXX' may be used without having been initialized */
#include <windows.h>
#include <ole2.h>
#pragma warning (pop)
#pragma warning (disable : 4001) /* nonstandard extension 'single line comment' was used */

#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1L
#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0L
#endif /* MSWin */

#endif /* _hosttype_H */
