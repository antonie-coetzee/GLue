#ifndef _extcode_H
#define _extcode_H
/**
	(c) Copyright 1990-2011 by National Instruments Corp.
	All rights reserved.

	@author National Instruments Corp.
	@file
	@brief  This document reflects what is published in the CIN manual.
	
	DO NOT MODIFY THIS FILE. It is automatically generated.
*/

#include "platdefines.h"
#include "fundtypes.h"

/* define this to keep C++ isms from littering the code */
#ifdef __cplusplus
	/* single instance extern C functions: */
	#define EXTERNC extern "C"
	/* single instance extern C VARIABLES (needed because MSVC unnecessarily
	   mangles global variable names): */
	#define EEXTERNC extern "C"
	/* begin of extern c block: */
	#define BEGINEXTERNC extern "C" {
	/* end of externc block */
	#define ENDEXTERNC }
	/* default function argument value */
	#define DEFAULTARG(a)= a
#else
	#define EXTERNC
	#define EEXTERNC extern
	#define BEGINEXTERNC
	#define ENDEXTERNC
	#define DEFAULTARG(a)
#endif /* __cplusplus */

/* Mark a function as using C calling conventions. */
#if !defined(_FUNCC)
	#if MSWin && (ProcessorType == kX86)
		#define _FUNCC __cdecl
	#else
		#define _FUNCC
	#endif
#endif

/* Mark a function as using standard (Pascal) calling conventions. */
#if !defined(_FUNCSTDCALL)
	#if MSWin && (ProcessorType == kX86)
		#define _FUNCSTDCALL __stdcall
	#else
		#define _FUNCSTDCALL
	#endif
#endif

/* FORCE_INLINE: cause a function to be inlined even when not optimizing */
#if MSWin
	#define FORCE_INLINE __forceinline
#elif Compiler == kGCC
	#define FORCE_INLINE inline __attribute__((always_inline))
#else
	#define FORCE_INLINE inline
#endif



/* Multi-threaded categorization tags. */
#ifndef TH_REENTRANT
	/* function is completely reentrant and calls only same */
	#define TH_REENTRANT
#endif
#ifndef TH_PROTECTED
	#define TH_PROTECTED  /* function uses mutex calls protect itself */
#endif
#ifndef TH_SINGLE_UI
	/* function is known to be used only from UI thread */
	#define TH_SINGLE_UI 
#endif
#ifndef TH_UNSAFE
	/* function is NOT thread safe and needs to be made so */
	#define TH_UNSAFE
#endif
	/* Variable tags */
#ifndef TH_READONLY
	/* (global) variable is never written to after startup */
	#define TH_READONLY
#endif

/* Explicitly define this if you want to use deprecated exported functions */
#ifndef USE_DEPRECATED_EXPORTS
	#define USE_DEPRECATED_EXPORTS 0
#endif

/*	Debugging ON section Begin	*/
#ifndef DBG
#define DBG 1
#endif
/*	Debugging ON section End	*/

/*	Debugging OFF section Begin
#undef DBG
#define DBG 0
	Debugging OFF section End	*/

#if Mac
	/* These must be defined before we start including Mac headers. */
	#ifndef ACCESSOR_CALLS_ARE_FUNCTIONS
		#define ACCESSOR_CALLS_ARE_FUNCTIONS 1
	#endif
	#ifndef OPAQUE_TOOLBOX_STRUCTS
		#define OPAQUE_TOOLBOX_STRUCTS 1
	#endif
	#include <Carbon/Carbon.h>
#endif // Mac

#include "lv_prolog.h"

#if !Mac
	/* These are native types on the Mac. */
	typedef uChar Str31[32];
	typedef uChar Str255[256];

#if USE_DEPRECATED_EXPORTS
	/* All of these are provided by Types.h on the Mac */
	typedef uChar		*StringPtr, **StringHandle;
	typedef const uChar	*ConstStringPtr;
#endif /* USE_DEPRECATED_EXPORTS */
#endif /* !Mac */

#if !Mac
	/** @brief Compatibility value for Mac-specific error code value.
	    @deprecated Unless using the Mac-specific OSStatus or OSErr type, you should use mgNoErr.

		A future release may place this definition behind the USE_DEPRECATED_EXPORTS guard.
	*/
	#define noErr mgNoErr
#endif /* !Mac */

typedef const uChar	ConstStr255[256];

/* Using 1 instead of 0 avoids a warning in GCC */
#ifdef __cplusplus
	#define __REINTERPRET_CAST(T,V) reinterpret_cast<T>(V)
#else
	#define __REINTERPRET_CAST(T,V) ((T)(V))
#endif
#define Offset(type, field)		((__REINTERPRET_CAST(size_t,(&__REINTERPRET_CAST(type*,1)->field)))-1)


/* Various mechanisms to define opaque types. */
#define LV_PRIVATE(T)	typedef struct T##_t { void *p; } *T
#define LV_PRIVATE_HANDLE(T)	 struct T##_t; typedef struct T##_t **T
#define LV_PRIVATE_POINTER(T)	 struct T##_t; typedef struct T##_t *T
#define LV_PRIVATE_POINTER_FWD(T)	typedef struct T##_t *T /* for forward declarations */

/** Legacy 16-bit Boolean type */
typedef uInt16 LVBooleanU16;
#define LVBooleanU16True	((LVBooleanU16)0x8000)
#define LVBooleanU16False	((LVBooleanU16)0x0000)

/** Standard Boolean type */
typedef uInt8 LVBoolean;
#define LVBooleanTrue		((LVBoolean)1)
#define LVBooleanFalse		((LVBoolean)0)
#define LVTRUE				LVBooleanTrue			/* for CIN users */
#define LVFALSE				LVBooleanFalse

/** @brief Opaque type used for various unique identifiers. */
typedef uInt32 MagicCookie;
#ifdef __cplusplus
#define kNotAMagicCookie static_cast<MagicCookie>(0)	/* canonical invalid MagicCookie value */
#else
#define kNotAMagicCookie ((MagicCookie)0)	/* canonical invalid MagicCookie value */
#endif

/** @brief The opaque type used by the Resource Manager. */
typedef MagicCookie RsrcFile;
#ifdef __cplusplus
#define kNotARsrcFile static_cast<RsrcFile>(kNotAMagicCookie)	/* canonical invalid RsrcFile */
#else
#define kNotARsrcFile ((RsrcFile)kNotAMagicCookie)	/* canonical invalid RsrcFile */
#endif

/** @brief Basic unsigned character pointer and handle types. */
typedef uChar		*UPtr, **UHandle;
typedef const uChar	*ConstUPtr, **ConstUHandle;
typedef uChar * const *UConstHandle;

/** @brief C-style (NULL-terminated) string types. */
typedef uChar		*CStr;
typedef const uChar	*ConstCStr;

/** @brief Pascal-style string types. 1-byte length followed by data. */
typedef uChar		*PStr, **PStrHandle;
typedef const uChar	*ConstPStr, **ConstPStrHandle;

typedef double floatNum;

/** @brief Concatenated Pascal string types. */
typedef struct {
	int32	cnt;		/* number of pascal strings that follow */
	uChar	str[1];		/* cnt bytes of concatenated pascal strings */
} CPStr, *CPStrPtr, **CPStrHandle;

/* Long Pascal-style string types. */
typedef struct {
	int32	cnt;		/* number of bytes that follow */
	uChar	str[1];		/* cnt bytes */
} LStr, *LStrPtr, **LStrHandle;
typedef LStr const* ConstLStrP;
typedef LStr const*const* ConstLStrH;

/** @brief Opaque type used by the Path Manager API. */
#ifdef __cplusplus
	class PathRef;
#else
	typedef struct PATHREF PathRef;
#endif /* __cplusplus */
typedef PathRef*		Path;
typedef const PathRef*	ConstPath;

/** @brief Translate an error code from any NI product into its text description.
	@param errCode the error code to translate
	@param errText receives the error description
	@return LVBooleanFalse iff error code is not found in the error code text files, LVBooleanTrue if the code was found. */
TH_REENTRANT EXTERNC LVBoolean _FUNCC NIGetOneErrorCode(int32 errCode, LStrHandle *errText);

/** @brief Manager error code type. */
typedef int32 MgErr;

/** @brief Manager error codes. */
enum {
	mgNoErr=0,
	mgArgErr=1,
	mFullErr,			/* Memory Mgr	2-3		*/
	mZoneErr,

	fEOF,				/* File Mgr		4-12	*/
	fIsOpen,
	fIOErr,
	fNotFound,
	fNoPerm,
	fDiskFull,
	fDupPath,
	fTMFOpen,
	fNotEnabled,

	rFNotFound,			/* Resource Mgr 13-15	*/
	rAddFailed,
	rNotFound,

	iNotFound,			/* Image Mgr	16-17	*/
	iMemoryErr,

	dPenNotExist,		/* Draw Mgr		18		*/

	cfgBadType,			/* Config Mgr	19-22	*/
	cfgTokenNotFound,
	cfgParseError,
	cfgAllocError,

	ecLVSBFormatError,	/* extCode Mgr	23		*/
	ecLVSBSubrError,	/* extCode Mgr	24		*/
	ecLVSBNoCodeError,	/* extCode Mgr	25		*/

	wNullWindow,		/* Window Mgr	26-27	*/
	wDestroyMixup,

	menuNullMenu,		/* Menu Mgr		28		*/

	pAbortJob,			/* Print Mgr	29-35	*/
	pBadPrintRecord,
	pDriverError,
	pWindowsError,
	pMemoryError,
	pDialogError,
	pMiscError,

	dvInvalidRefnum,	/* Device Mgr	36-41	*/
	dvDeviceNotFound,
	dvParamErr,
	dvUnitErr,
	dvOpenErr,
	dvAbortErr,

	bogusError,			/* generic error 42 */
	cancelError,		/* cancelled by user 43 */

	OMObjLowErr,		/* object message dispatcher errors 44-52 */
	OMObjHiErr,
	OMObjNotInHeapErr,
	OMOHeapNotKnownErr,
	OMBadDPIdErr,
	OMNoDPinTabErr,
	OMMsgOutOfRangeErr,
	OMMethodNullErr,
	OMUnknownMsgErr,

	mgNotSupported,

	ncBadAddressErr,		/* Net Connection errors 54-66 */
	ncInProgress,
	ncTimeOutErr,
	ncBusyErr,
	ncNotSupportedErr,
	ncNetErr,
	ncAddrInUseErr,
	ncSysOutOfMem,
	ncSysConnAbortedErr,	/* 62 */
	ncConnRefusedErr,
	ncNotConnectedErr,
	ncAlreadyConnectedErr,
	ncConnClosedErr,		/* 66 */

	amInitErr,				/* (Inter-)Application Message Manager 67- */

	occBadOccurrenceErr,	/* 68  Occurrence Mgr errors */
	occWaitOnUnBoundHdlrErr,
	occFunnyQOverFlowErr,

	fDataLogTypeConflict,	/* 71 */
	ecLVSBCannotBeCalledFromThread, /* ExtCode Mgr	72*/
	amUnrecognizedType,
	mCorruptErr,
	ecLVSBErrorMakingTempDLL,
	ecLVSBOldCIN,			/* ExtCode Mgr	76*/

	dragSktNotFound,		/* Drag Manager 77 - 80*/
	dropLoadErr,
	oleRegisterErr,
	oleReleaseErr,

	fmtTypeMismatch,		/* String processing (printf, scanf) errors */
	fmtUnknownConversion,
	fmtTooFew,
	fmtTooMany,
	fmtScanError,

	ecLVSBFutureCIN,		/* ExtCode Mgr	86*/

	lvOLEConvertErr,
	rtMenuErr,
	pwdTampered,			/* Password processing */
	LvVariantAttrNotFound,		/* LvVariant attribute not found 90-91*/
	LvVariantTypeMismatch,		/* Cannot convert to/from type */

	axEventDataNotAvailable,	/* Event Data Not Available 92-96*/
	axEventStoreNotPresent,		/* Event Store Not Present */
	axOccurrenceNotFound,		/* Occurence Not Found */
	axEventQueueNotCreated,		/* Event Queue not created */
	axEventInfoNotAvailable,	/* Event Info is not available */

	oleNullRefnumPassed,		/* Refnum Passed is Null */

	omidGetClassGUIDErr,		/* Error retrieving Class GUID from OMId 98-100*/
	omidGetCoClassGUIDErr,		/* Error retrieving CoClass GUID from OMId */
	omidGetTypeLibGUIDErr,		/* Error retrieving TypeLib GUID from OMId */

	appMagicBad,				/* bad built application magic bytes */

	iviInvalidDowncast,         /* IVI Invalid downcast*/
	iviInvalidClassSesn,		/* IVI No Class Session Opened */

	maxErr,						/* max manager 104-107 */
	maxConfigErr,				/* something not right with config objects */
	maxConfigLoadErr,			/* could not load configuration */
	maxGroupNotSupported,

	ncSockNotMulticast,			/* net connection multicast specific errors 108-112 */
	ncSockNotSinglecast,
	ncBadMulticastAddr,
	ncMcastSockReadOnly,
	ncMcastSockWriteOnly,

	ncDatagramMsgSzErr,			/* net connection Datagram message size error 113 */

	bufferEmpty,				/* CircularLVDataBuffer (queues/notifiers) */
	bufferFull,					/* CircularLVDataBuffer (queues/notifiers) */
	dataCorruptErr,				/* error unflattening data */

	requireFullPathErr,			/* supplied folder path where full file path is required  */
	folderNotExistErr,			/* folder doesn't exist */

	ncBtInvalidModeErr,			/* invalid Bluetooth mode 119 */
	ncBtSetModeErr,				/* error setting Bluetooth mode 120 */

	mgBtInvalidGUIDStrErr,		/* The GUID string is invalid 121 */

	rVersInFuture,			/* Resource file is a future version 122 */

	mgErrTimeOut,			/* General-purpose timeout 123 */
	mgErrStringCannotContainNull,	/* An LStr contained a null character in a place that does not support null characters 124 */
	mgErrStackOverflow,			/* Stack overflow detected 125 */

	mgErrSentinel,	// 126

	mgPrivErrBase = 500,	/* Start of Private Errors */
	mgPrivErrLast = 599,	/* Last allocated in Error DB */


	kAppErrorBase = 1000,	/* Start of application errors */
	kAppLicenseErr = 1380,	/* Failure to check out license error */
	kAppCharsetConvertErr =1396, /* could not convert text from charset to charset */
	kAppErrorLast = 1399	/* Last allocated in Error DB */
};

typedef MagicCookie Occurrence;
#ifndef kNonOccurrence
	#ifdef __cplusplus
		#define kNonOccurrence static_cast<Occurrence>(kNotAMagicCookie)
	#else
		#define kNonOccurrence ((Occurrence)(kNotAMagicCookie))
	#endif
#endif

TH_PROTECTED EXTERNC MgErr _FUNCC Occur(Occurrence o);

/* forward definitions of ATime128 (time stamp) for DLLs and CINs */
#ifdef __cplusplus
class ATime128;
#else
typedef struct ATIME128 ATime128;
#endif /* __cplusplus */
typedef ATime128* ATime128Ptr;

#ifdef __cplusplus
class LvVariant;
#else
typedef struct LVVARIANT LvVariant;
#endif
typedef LvVariant* LvVariantPtr;

/* forward definitions of HWAVEFORM and HWAVES for DLLs and CINs */
#if !defined(__cplusplus) || defined(NICOM_C_INTERFACES)
typedef struct IWaveform IWaveform;
typedef struct IWaves IWaves;
#else
class IWaveform;
class IWaves;
#endif

typedef IWaveform* HWAVEFORM;
typedef IWaves* HWAVES;

/** Used for converting from NICOM to different flavors of LV-WDT */
enum {
	kWDTUniform =0L,	/*< Uniform Flt64 WDT */
	kArrayWDTUniform	/*< Array of uniform flt64 WDT */
};

/*** Numeric Support Functions and Utilities ***/
/** @brief These values describe various scalar numeric types. */
typedef enum {	iB=1, iW, iL, iQ, uB, uW, uL, uQ, fS, fD, fX, cS, cD, cX } NumType;

#if defined(__cplusplus)
#define _NumericTypeCast_(T,v)	static_cast<T>(v)
#else
#define _NumericTypeCast_(T,v)	((T)(v))
#endif
#define _NumericTypeCastTwice_(T1,T2,v) _NumericTypeCast_(T1,_NumericTypeCast_(T2,v))

#define HiNibble(x)		_NumericTypeCast_(uInt8,((x)>>4) & 0x0F)
#define LoNibble(x)		_NumericTypeCast_(uInt8,(x) & 0x0F)
#define HiByte(x)		_NumericTypeCast_(int8,_NumericTypeCast_(int16,x)>>8)
#define LoByte(x)		_NumericTypeCast_(int8,x) 
#define Word(hi,lo)		((_NumericTypeCast_(int16,hi)<<8) | _NumericTypeCastTwice_(int16, uInt8, lo))
#define Hi16(x)			_NumericTypeCast_(int16, _NumericTypeCast_(int32, x) >> 16)
#define Lo16(x)			_NumericTypeCastTwice_(int16, int32, x)
#define Long(hi,lo)		((_NumericTypeCast_(int32,hi)<<16) | _NumericTypeCastTwice_(int32, uInt16,lo))
#define Hi32(x)			_NumericTypeCast_(int32,_NumericTypeCast_(int64,x)>>32)
#define Lo32(x)			_NumericTypeCastTwice_(int32, int64, x)

#define Cat2Chrs(c1,c2) \
		(	((_NumericTypeCastTwice_(int16,uInt8,c1))<< 8) \
		|	((_NumericTypeCastTwice_(int16,uInt8,c2))    ) )
#define Cat4Chrs(c1,c2,c3,c4) \
		(	((_NumericTypeCastTwice_(int32,uInt8,c1))<<24) \
		|	((_NumericTypeCastTwice_(int32,uInt8,c2))<<16) \
		|	((_NumericTypeCastTwice_(int32,uInt8,c3))<< 8) \
		|	((_NumericTypeCastTwice_(int32,uInt8,c4))    ) )
#define Cat8Chrs(c1,c2,c3,c4, c5, c6, c7, c8) \
		(	((_NumericTypeCastTwice_(int64,uInt8,c1))<<56) \
		|	((_NumericTypeCastTwice_(int64,uInt8,c2))<<48) \
		|	((_NumericTypeCastTwice_(int64,uInt8,c3))<<40) \
		|	((_NumericTypeCastTwice_(int64,uInt8,c4))<<32) \
		|	((_NumericTypeCastTwice_(int64,uInt8,c5))<<24) \
		|	((_NumericTypeCastTwice_(int64,uInt8,c6))<<16) \
		|	((_NumericTypeCastTwice_(int64,uInt8,c7))<< 8) \
		|	((_NumericTypeCastTwice_(int64,uInt8,c8))    ) )
#ifdef NIDL
	#define RTToL(c1,c2,c3,c4)	0
#else
#if NI_BIG_ENDIAN
	#define RTToL(c1,c2,c3,c4)	Cat4Chrs(c1,c2,c3,c4)
	#define RTToW(c1,c2)		Cat2Chrs(c1,c2)
	#define IntFrom4Chars(s)	_NumericTypeCast_(int32,s)
#else
	#define RTToL(c1,c2,c3,c4)	Cat4Chrs(c4,c3,c2,c1)
	#define RTToW(c1,c2)		Cat2Chrs(c2,c1)
	#define IntFrom4Chars(s)	((_NumericTypeCast_(int32,s)<<24)|((_NumericTypeCast_(int32,s)&0x0000ff00)<<8)|(_NumericTypeCast_(uInt32,s)>>24)|((_NumericTypeCast_(int32,s)&0x00ff0000)>>8))
#endif /* NI_BIG_ENDIAN */
#endif /* NIDL */

#if !Palm
/** @brief Computes absolute value of a number.
	@param n is an integer argument
	@return the absolute value of n

	@deprecated Call abs() instead.
*/
TH_REENTRANT EXTERNC int32 _FUNCC Abs(int32);

/** @brief Computes min of two numbers.
	@param n the first integer argument
	@param m the second integer argument
	@return the min of n and m */
TH_REENTRANT EXTERNC int32 _FUNCC Min(int32 n, int32 m);

/** @brief Computes max of two numbers.
	@param n the first integer argument
	@param m the second integer argument
	@return the max of n and m */
TH_REENTRANT EXTERNC int32 _FUNCC Max(int32 n, int32 m);
#endif /* !Palm */

#if (ProcessorType==kX86) || (ProcessorType==kM68000)
	#define UseGetSetIntMacros	1
#else
	#define UseGetSetIntMacros	0
#endif

#if UseGetSetIntMacros
	#define GetAWord(p)		(*(int16*)(p))
	#define SetAWord(p,x)	(*(int16*)(p)= x)
	#define GetALong(p)		(*(int32*)(p))
	#define SetALong(p,x)	(*(int32*)(p)= x)
#else
TH_REENTRANT EXTERNC int32 _FUNCC	GetALong(const void *);
TH_REENTRANT EXTERNC int32 _FUNCC	SetALong(void *, int32);
TH_REENTRANT EXTERNC int16 _FUNCC	GetAWord(const void *);
TH_REENTRANT EXTERNC int16 _FUNCC	SetAWord(void *, int16);
#endif

#if !Palm
/* Avoid conflict with functions in LabWindows CVI that can arise for LabVIEW DLLs used in CVI projects. */
#ifndef _CVI_
/** @brief Coerce a value to be in the range [low,high].
	@param i the value to test for coersion
	@param low the lower limit
	@param high the upper limit
	@return the coerced value */
TH_REENTRANT EXTERNC int32 _FUNCC Pin(int32 i, int32 low, int32 high);	
#endif /* _CVI_ */
#endif /* !Palm */

/** @brief Generate a random number between 0 and 1.
	@param xp receives the randomly generated value */
TH_PROTECTED EXTERNC void _FUNCC RandomGen(float64 *xp);

/*** C-style String Support Functions ***/
#if !Palm
TH_REENTRANT EXTERNC Bool32 _FUNCC IsDigit(uChar);
TH_REENTRANT EXTERNC Bool32 _FUNCC IsAlpha(uChar);
TH_REENTRANT EXTERNC Bool32 _FUNCC IsPunct(uChar);
TH_REENTRANT EXTERNC Bool32 _FUNCC IsLower(uChar);
TH_REENTRANT EXTERNC Bool32 _FUNCC IsUpper(uChar);
TH_REENTRANT EXTERNC int32 _FUNCC ToUpper(int32);
TH_REENTRANT EXTERNC int32 _FUNCC ToLower(int32);
TH_REENTRANT EXTERNC uChar _FUNCC HexChar(int32);
TH_REENTRANT EXTERNC int32 _FUNCC StrLen(const uChar *);
TH_REENTRANT EXTERNC int32 _FUNCC StrCat(CStr, ConstCStr);
TH_REENTRANT EXTERNC CStr _FUNCC StrCpy(CStr, ConstCStr);
TH_REENTRANT EXTERNC CStr _FUNCC StrNCpy(CStr, ConstCStr, const size_t);
TH_REENTRANT EXTERNC int32 _FUNCC StrCmp(ConstCStr, ConstCStr);
TH_REENTRANT EXTERNC int32 _FUNCC StrNCmp(ConstCStr, ConstCStr, const size_t);
TH_REENTRANT EXTERNC int32 _FUNCC StrNCaseCmp(ConstCStr, ConstCStr, const size_t);

/** @brief Search a string for a substring.
	@param s the string to search
	@param r the string to search for within s
	@return nonzero value if r is found within s
	
	@note Comparisons are case-insensitive.
	@note Whitespace in s is ignored.
*/
TH_REENTRANT EXTERNC int32 _FUNCC StrIStr(ConstCStr s, ConstCStr r);
#endif /* !Palm */

/*** Pascal-Style String Support Functions ***/
#if __cplusplus
inline uChar*       PStrBuf(const PStr b)		{ return &(b[1]); }
inline uChar&       PStrLen(const PStr b)		{ return b[0]; }	// # of chars in string
inline const uChar* PStrBuf(const ConstPStr b)	{ return &(b[1]); }
inline uChar        PStrLen(const ConstPStr b)	{ return b[0]; }	// # of chars in string
inline uInt16       PStrSize(const ConstPStr b)	{ return static_cast<uInt16>(b?PStrLen(b)+1:0); } // # of bytes including length;
																					   // The return type is uInt16, not uChar. It cannot be uChar since
																					   //	a) Maximum PStr length is 255
																					   //	b) PStrSize is length plus 1
																					   //	c) 255 + 1 = 256
																					   //	d) 256 is greater than the maximum value of a uChar (255)
#else
#define PStrBuf(b)		(&((PStr)(b))[1])
#define PStrLen(b)		(((PStr)(b))[0])				/* # of chars in string */
#define PStrSize(b)		(b?PStrLen(b)+1:0)				/* # of bytes including length */
#endif /* __cplusplus */

TH_REENTRANT EXTERNC int32 _FUNCC PStrCat(PStr, ConstPStr);
TH_REENTRANT EXTERNC PStr _FUNCC PStrCpy(PStr, ConstPStr);
TH_REENTRANT EXTERNC PStr _FUNCC PStrNCpy(PStr, ConstPStr, int32);
TH_REENTRANT EXTERNC int32 _FUNCC PStrCmp(ConstPStr, ConstPStr);
TH_REENTRANT EXTERNC int32 _FUNCC PPStrCmp(ConstPStr*, ConstPStr*);
TH_REENTRANT EXTERNC int32 _FUNCC PStrCaseCmp(ConstPStr, ConstPStr);
TH_REENTRANT EXTERNC int32 _FUNCC PPStrCaseCmp(ConstPStr*, ConstPStr*);
TH_REENTRANT EXTERNC int32 _FUNCC PToCStr(ConstPStr, CStr);
TH_REENTRANT EXTERNC PStr _FUNCC PStrDup(ConstPStr buf);
TH_REENTRANT EXTERNC int32 _FUNCC CToPStr(ConstCStr, PStr);

/*** Long Pascal-Style String Support Functions ***/

/* Helpers for LStr handles that allow for empty strings encoded as NULL handles.
   Use LHStr macros instead of passing a *h to LStrBuf/Len/Size for right hand side refs. */
#define LHStrPtr(sh)	((sh)?*(sh):NULL)				/* Get LStr* (produces NULL ptr for NULL handle */
#define LHStrBuf(sh)	((sh)?(&(*(sh))->str[0]):NULL)	/* same as LStrBuf, but on string handle */
#define LHStrLen(sh)	((sh)?((*(sh))->cnt):0)			/* same as LStrLen, but on string handle */
#define LHStrSize(sh)	(LHStrLen(sh)+sizeof(int32))	/* same as LStrSize, but on string handle */

#define LStrBuf(sp)	(&((sp))->str[0])				/* pointer to first char of string */
#define LStrLen(sp)	(((sp))->cnt)					/* # of chars in string */
#define LStrSize(sp)	(LStrLen(sp)+sizeof(int32))		/* # of bytes including length */

TH_REENTRANT EXTERNC int32 _FUNCC LStrCmp(ConstLStrP l1, ConstLStrP l2);
TH_REENTRANT EXTERNC int32 _FUNCC LStrCaseCmp(ConstLStrP l1, ConstLStrP l2);
TH_REENTRANT EXTERNC int32 _FUNCC PtrLenStrCmp(const uChar *s1, int32 s1len, const uChar *s2, int32 s2len);
TH_REENTRANT EXTERNC int32 _FUNCC PtrLenStrCaseCmp(const uChar *s1, int32 s1len, const uChar *s2, int32 s2len);
TH_REENTRANT EXTERNC int32 _FUNCC LHStrCmp(ConstLStrH lh1, ConstLStrH lh2);
TH_REENTRANT EXTERNC int32 _FUNCC LHStrCaseCmp(ConstLStrH lh1, ConstLStrH lh2);

TH_REENTRANT EXTERNC uInt32 _FUNCC LToCStrN(ConstLStrP source, CStr dest, uInt32 destSize);
TH_REENTRANT EXTERNC int32 _FUNCC LToPStr(ConstLStrP source, PStr dest);
TH_REENTRANT EXTERNC int32 _FUNCC PToLStr(ConstPStr source, LStrPtr dest);
TH_REENTRANT EXTERNC MgErr _FUNCC PStrToDSLStr(PStr buf, LStrHandle *lStr); /* deprecated */

#if USE_DEPRECATED_EXPORTS
/** @brief Convert a long Pascal-style string to a C-style string.
	@param lstr the long pascal string to convert
	@param cstr receives the contents of the Lstr
	@return length (in bytes) of converted string
	@pre cstr is large enough to contain lstr ((LStrLen(lstr) + 1) or more bytes)
	@deprecated Use LToCStrN() instead.
*/
TH_REENTRANT EXTERNC int32 _FUNCC LToCStr(ConstLStrP lstr, CStr cstr);
#endif /* USE_DEPRECATED_EXPORTS */

/*** Concatenated Pascal String Support Functions ***/
#define CPStrLen		LStrLen			/* concatenated Pascal vs. LabVIEW strings */
#define CPStrBuf		LStrBuf			/* concatenated Pascal vs. LabVIEW strings */

TH_REENTRANT EXTERNC int32 _FUNCC CPStrSize(CPStrPtr);
TH_REENTRANT EXTERNC int32 _FUNCC CPStrCmp(CPStrPtr, CPStrPtr);
TH_REENTRANT EXTERNC MgErr _FUNCC CPStrInsert(CPStrHandle, ConstPStr, int32);
TH_REENTRANT EXTERNC void _FUNCC CPStrRemove(CPStrHandle, int32);
TH_REENTRANT EXTERNC PStr _FUNCC CPStrIndex(CPStrHandle, int32);
TH_REENTRANT EXTERNC MgErr _FUNCC CPStrReplace(CPStrHandle, ConstPStr, int32);

TH_REENTRANT EXTERNC MgErr _FUNCC DbgPrintf(const char *buf, ...);

/** Refer to SPrintf() */
TH_PROTECTED EXTERNC int32 DBPrintf(const char *fmt, ...);

TH_REENTRANT EXTERNC int32 _FUNCC SPrintf(CStr buf, ConstCStr fmt, ...);
TH_REENTRANT EXTERNC int32 _FUNCC SPrintfp(CStr buf, ConstPStr fmt, ...);

TH_REENTRANT EXTERNC MgErr _FUNCC LStrPrintf(LStrHandle str, CStr fmt, ...);

TH_REENTRANT EXTERNC int32 _FUNCC PPrintf(PStr, ConstCStr, ...);
TH_REENTRANT EXTERNC int32 _FUNCC PPrintfp(PStr, ConstPStr, ...);

#ifndef NIDL
/*** Sort / Search Utilities ***/
/** @brief Comparison function pointer type definiton. */
typedef int32 (_FUNCC *CompareProcPtr)(const void *lhs, const void *rhs);

/* Avoid conflict with functions in LabWindows CVI that can arise for LabVIEW DLLs used in CVI projects. */
#ifndef _CVI_
TH_REENTRANT EXTERNC int32 _FUNCC BinSearch(const void*, int32, int32, const void*, CompareProcPtr);
#endif /* _CVI_ */
TH_REENTRANT EXTERNC void _FUNCC QSort(void*, int32, int32, CompareProcPtr);
#endif /* NIDL */

/*** Time Functions ***/
/** @brief Date/time record. */
typedef struct {
	int32	sec;	/* 0:59 */
	int32	min;	/* 0:59 */
	int32	hour;	/* 0:23 */
	int32	mday;	/* day of the month, 1:31 */
	int32	mon;	/* month of the year, 1:12 */
	int32	year;	/* year, 1904:2040 */
	int32	wday;	/* day of the week, 1:7 for Sun:Sat */
	int32	yday;	/* day of year (julian date), 1:366 */
	int32	isdst;	/* 1 if daylight savings time */
} DateRec;

TH_REENTRANT EXTERNC void _FUNCC SecsToDate(uInt32, DateRec*);
TH_REENTRANT EXTERNC uInt32 _FUNCC DateToSecs(DateRec*);
TH_REENTRANT EXTERNC uInt32 _FUNCC TimeInSecs(void);
TH_UNSAFE EXTERNC CStr _FUNCC DateCString(uInt32, int32);
TH_UNSAFE EXTERNC CStr _FUNCC TimeCString(uInt32, int32);
TH_UNSAFE EXTERNC CStr _FUNCC ASCIITime(uInt32);

TH_REENTRANT EXTERNC uInt32 _FUNCC MilliSecs();

/*** The Memory Manager ***/
/*
	For parameters to the memory manager functions below
	p means pointer
	h means handle
	ph means pointer to handle
*/
TH_REENTRANT EXTERNC UHandle _FUNCC DSNewHandle(size_t);
TH_REENTRANT EXTERNC UHandle _FUNCC DSNewHClr(size_t);
TH_REENTRANT EXTERNC UPtr _FUNCC DSNewPtr(size_t);
TH_REENTRANT EXTERNC UPtr _FUNCC DSNewPClr(size_t);
TH_REENTRANT EXTERNC MgErr _FUNCC DSCopyHandle(void *ph, const void *hsrc);
TH_REENTRANT EXTERNC MgErr _FUNCC DSSetHandleSize(void *h, size_t);
TH_REENTRANT EXTERNC MgErr _FUNCC DSSetHSzClr(void *h, size_t);
TH_REENTRANT EXTERNC MgErr _FUNCC DSDisposeHandle(void *h);
TH_REENTRANT EXTERNC MgErr _FUNCC DSDisposePtr(void *p);

#if !Palm
TH_REENTRANT EXTERNC MgErr _FUNCC DSCheckHandle(const void *h);
TH_REENTRANT EXTERNC MgErr _FUNCC DSCheckPtr(void *p);
TH_REENTRANT EXTERNC size_t _FUNCC DSGetHandleSize(const void *h);
TH_REENTRANT EXTERNC UHandle _FUNCC DSRecoverHandle(void *p);
TH_REENTRANT EXTERNC MgErr _FUNCC DSHeapCheck(Bool32);
TH_REENTRANT EXTERNC size_t _FUNCC DSMaxMem(void);

/** @brief Describes memory statistics. */
typedef struct {
	int32 totFreeSize;
	int32 maxFreeSize;
	int32 nFreeBlocks;
	size_t totAllocSize;
	size_t maxAllocSize;
	int32 nPointers;
	int32 nUnlockedHdls;
	int32 nLockedHdls;
	int32 reserved[4];
} MemStatRec;

TH_REENTRANT EXTERNC MgErr _FUNCC DSMemStats(MemStatRec *msrp);
#endif /* !Palm */

#if !Palm
TH_REENTRANT EXTERNC void _FUNCC ClearMem(void*, size_t);
TH_REENTRANT EXTERNC void _FUNCC MoveBlock(const void *src, void *dest, size_t); /* accepts zero bytes */
/* Avoid conflict with functions in LabWindows CVI that can arise for LabVIEW DLLs used in CVI projects. */
#ifndef _CVI_
TH_REENTRANT EXTERNC void _FUNCC SwapBlock(void *src, void *dest, size_t);
#endif /* _CVI_ */
#endif /* !Palm */
TH_REENTRANT EXTERNC int32 _FUNCC BlockCmp(const void * p1, const void * p2, size_t n);

#if USE_DEPRECATED_EXPORTS
#if !Palm
TH_REENTRANT EXTERNC MgErr _FUNCC AZCheckHandle(const void *h);
TH_REENTRANT EXTERNC MgErr _FUNCC AZCheckPtr(void *p);
TH_REENTRANT EXTERNC MgErr _FUNCC AZDisposeHandle(void *h);
TH_REENTRANT EXTERNC MgErr _FUNCC AZDisposePtr(void *p);
TH_REENTRANT EXTERNC size_t _FUNCC AZGetHandleSize(const void *h);
TH_REENTRANT EXTERNC MgErr _FUNCC AZHLock(void *h);
TH_REENTRANT EXTERNC MgErr _FUNCC AZHUnlock(void *h);
TH_REENTRANT EXTERNC void _FUNCC AZHPurge(void *h);
TH_REENTRANT EXTERNC void _FUNCC AZHNoPurge(void *h);
TH_REENTRANT EXTERNC UHandle _FUNCC AZNewHandle(size_t);
TH_REENTRANT EXTERNC UHandle _FUNCC AZNewHClr(size_t);
TH_REENTRANT EXTERNC UPtr _FUNCC AZNewPtr(size_t);
TH_REENTRANT EXTERNC UPtr _FUNCC AZNewPClr(size_t);
TH_REENTRANT EXTERNC UHandle _FUNCC AZRecoverHandle(void *p);
TH_REENTRANT EXTERNC MgErr _FUNCC AZSetHandleSize(void *h, size_t);
TH_REENTRANT EXTERNC MgErr _FUNCC AZSetHSzClr(void *h, size_t);
TH_REENTRANT EXTERNC int32 _FUNCC AZHeapCheck(Bool32);
TH_REENTRANT EXTERNC size_t _FUNCC AZMaxMem(void);
TH_REENTRANT EXTERNC MgErr _FUNCC AZMemStats(MemStatRec *msrp);
TH_REENTRANT EXTERNC MgErr _FUNCC AZSetHandleFromPtr(void *ph, const void *psrc, size_t n);
#endif /* !Palm */
#endif /* USE_DEPRECATED_EXPORTS */

/*** The File Manager ***/

/** @brief Initial enumeration type for file types.
	The enumeration is used to increase type safety but does not list all possible values.
*/
typedef enum _FMFileType {
	kInvalidType	=0,
	kUnknownFileType=RTToL('?','?','?','?'),
	kTextFileType	=RTToL('T','E','X','T'),
	/** Typical directory types */
	kHardDiskDirType=RTToL('h','d','s','k'),
	kFloppyDirType	=RTToL('f','l','p','y'),
	kNetDriveDirType=RTToL('s','r','v','r')
}FMFileType;

/** @brief Type for file creator codes.
	The enumeration is used to increase type safety but does not list all possible values.*/
typedef enum  {
	kInvalidCreator	=0,
	kUnknownCreator =RTToL('?','?','?','?'),
	/** LabVIEW creator type */
	kLVCreatorType	=RTToL('L','B','V','W')
}FMFileCreator;

/** @brief Opaque type used by the File Manager API. */
LV_PRIVATE(File);

/** Used with FListDir */
typedef struct {
	int32 flags;
	FMFileType type;
} FMListDetails;

/** Type Flags used with FMListDetails */
#define kIsFile				0x01
#define kRecognizedType		0x02
#define kIsLink				0x04
#define kFIsInvisible		0x08
#define kIsTopLevelVI		0x10	/**< Used only for VIs in archives */
#define kErrGettingType		0x20	/**< error occurred getting type info */
#if Mac
#define kFIsStationery		0x40
#endif /* Mac */

typedef CPStr FDirEntRec, *FDirEntPtr, **FDirEntHandle; /**< directory list record */

/** @brief Descriptive information about a file. */
typedef struct {			/**< file/directory information record */
	FMFileType type;		/**< system specific file type-- 0 for directories */
	FMFileCreator creator;	/**< system specific file creator-- 0 for directories */
	int32	permissions;	/**< system specific file access rights */
	int32	size;			/**< file size in bytes (data fork on Mac) or entries in folder */
	int32	rfSize;			/**< resource fork size (on Mac only) */
	uInt32	cdate;			/**< creation date */
	uInt32	mdate;			/**< last modification date */
	Bool32	folder;			/**< indicates whether path refers to a folder */
	Bool32	isInvisible; /**< indicates whether the file is visible in File Dialog */
	struct {
		int16 v;
		int16 h;
	} location;			/**< system specific geographical location */
	Str255	owner;			/**< owner (in pascal string form) of file or folder */
	Str255	group;			/**< group (in pascal string form) of file or folder */
} FInfoRec, *FInfoPtr;

TH_REENTRANT EXTERNC MgErr _FUNCC FGetInfo(ConstPath path, FInfoPtr infop);
TH_REENTRANT EXTERNC MgErr _FUNCC FSetInfo(ConstPath path, FInfoPtr infop);

/** @brief Used for FGetInfo, 64-bit version */
typedef uInt32	FGetInfoWhich;
enum {
	kFGetInfoType			= 1L << 0,
	kFGetInfoCreator		= 1L << 1,
	kFGetInfoPermissions	= 1L << 2,
	kFGetInfoSize			= 1L << 3,
	kFGetInfoRFSize			= 1L << 4,
	kFGetInfoCDate			= 1L << 5,
	kFGetInfoMDate			= 1L << 6,
	kFGetInfoFolder			= 1L << 7,
	kFGetInfoIsInvisible	= 1L << 8,
	kFGetInfoLocation		= 1L << 9,
	kFGetInfoOwner			= 1L << 10,
	kFGetInfoGroup			= 1L << 11,
	kFGetInfoAll			= 0xEFFFFFFFL
};

typedef struct {			/**< file/directory information record */
	FMFileType type;		/**< system specific file type-- 0 for directories */
	FMFileCreator creator;	/**< system specific file creator-- 0 for directories */
	int32	permissions;	/**< system specific file access rights */
	int64	size;			/**< file size in bytes (data fork on Mac) or entries in folder */
	int64	rfSize;			/**< resource fork size (on Mac only) */
	uInt32	cdate;			/**< creation date */
	uInt32	mdate;			/**< last modification date */
	Bool32	folder;			/**< indicates whether path refers to a folder */
	Bool32	isInvisible; /**< indicates whether the file is visible in File Dialog */
	struct {
		int16 v;
		int16 h;
	} location;			/**< system specific geographical location */
	Str255	owner;			/**< owner (in pascal string form) of file or folder */
	Str255	group;			/**< group (in pascal string form) of file or folder */
} FInfoRec64, *FInfo64Ptr;

TH_REENTRANT EXTERNC MgErr _FUNCC FGetInfo64(ConstPath path, FInfo64Ptr infop, FGetInfoWhich which DEFAULTARG(kFGetInfoAll));
TH_REENTRANT EXTERNC MgErr _FUNCC FSetInfo64(ConstPath path, FInfo64Ptr infop);

/** Used with FExists */
enum {
	kFNotExist = 0L,
	kFIsFile,
	kFIsFolder
};

/** open modes */
enum { openReadWrite, openReadOnly, openWriteOnly, openWriteOnlyTruncate,
	   openReadWriteUnbuffered, openReadOnlyUnbuffered, openWriteOnlyUnbuffered, openWriteOnlyTruncateUnbuffered };
/** deny modes */
enum { denyReadWrite, denyWriteOnly, denyNeither };
/** seek modes */
enum { fStart=1, fEnd, fCurrent };

TH_REENTRANT EXTERNC MgErr _FUNCC FCreate(File *fdp, ConstPath path, int32 access, int32 openMode, int32 denyMode, PStr group);
TH_REENTRANT EXTERNC MgErr _FUNCC FCreateAlways(File *fdp, ConstPath path, int32 access, int32 openMode, int32 denyMode, PStr group);
TH_REENTRANT EXTERNC MgErr _FUNCC FMOpen(File *fdp, ConstPath path, int32 openMode, int32 denyMode);
TH_REENTRANT EXTERNC MgErr _FUNCC FMClose(File fd);
TH_REENTRANT EXTERNC MgErr _FUNCC FMSeek(File fd, int32 ofst, int32 mode);
TH_REENTRANT EXTERNC MgErr _FUNCC FMSeek64(File fd, int64 ofst, int32 mode);
TH_REENTRANT EXTERNC MgErr _FUNCC FMTell(File fd, int32 *ofstp);
TH_REENTRANT EXTERNC MgErr _FUNCC FMTell64(File fd, int64 *ofstp);
TH_REENTRANT EXTERNC MgErr _FUNCC FGetEOF(File fd, int32 *sizep);
TH_REENTRANT EXTERNC MgErr _FUNCC FGetEOF64(File fd, int64 *sizep);
TH_REENTRANT EXTERNC MgErr _FUNCC FSetEOF(File fd, int32 size);
TH_REENTRANT EXTERNC MgErr _FUNCC FSetEOF64(File fd, int64 size);
TH_REENTRANT EXTERNC MgErr _FUNCC FMRead(File fd, int32 inCount, int32 *outCountp, UPtr buffer);
TH_REENTRANT EXTERNC MgErr _FUNCC FMWrite(File fd, int32 inCount, int32 *outCountp, ConstUPtr buffer);
TH_REENTRANT EXTERNC int32 _FUNCC FPrintf(File, ConstCStr, ...);
TH_REENTRANT EXTERNC MgErr _FUNCC FPrintfWErr(File fd, ConstCStr fmt, ...);
TH_REENTRANT EXTERNC MgErr _FUNCC FLockOrUnlockRange(File, int32, int32, int32, Bool32);
TH_REENTRANT EXTERNC MgErr _FUNCC FFlush(File fd);
TH_REENTRANT EXTERNC int32 _FUNCC FExists(ConstPath path);
TH_REENTRANT EXTERNC MgErr _FUNCC FGetAccessRights(ConstPath path, PStr owner, PStr group, int32 *permPtr);
TH_REENTRANT EXTERNC MgErr _FUNCC FSetAccessRights(ConstPath path, PStr owner, PStr group, int32 *permPtr);
TH_REENTRANT EXTERNC MgErr _FUNCC FMove(ConstPath oldPath, ConstPath newPath);
TH_REENTRANT EXTERNC MgErr _FUNCC FCopy(ConstPath oldPath, ConstPath newPath);
TH_REENTRANT EXTERNC MgErr _FUNCC FRemove(ConstPath path);
TH_REENTRANT EXTERNC MgErr _FUNCC FRemoveToRecycle(ConstPath path, Bool32 showConfirmDlgs);
TH_REENTRANT EXTERNC MgErr _FUNCC FNewDir(ConstPath path, int32 permissions);

/** Used for FGetVolInfo */
typedef struct {
	uInt32	size;			/**< size in bytes of a volume */
	uInt32	used;			/**< number of bytes used on volume */
	uInt32	free;			/**< number of bytes available for use on volume */
} VInfoRec;
TH_REENTRANT EXTERNC MgErr _FUNCC FGetVolInfo(Path, VInfoRec*);
TH_REENTRANT EXTERNC MgErr _FUNCC FMGetVolInfo(ConstPath, float64*, float64*);
TH_REENTRANT EXTERNC MgErr _FUNCC FMGetVolInfo64(ConstPath, uInt64*, uInt64*, uInt32*);
TH_REENTRANT EXTERNC MgErr _FUNCC FMGetVolPath(ConstPath, Path*);

TH_REENTRANT EXTERNC MgErr _FUNCC FListDir(ConstPath path, FDirEntHandle list, FMListDetails **);

/*** The Path Manager ***/

/* FileNameCmp compares two PStr's with the same case-sensitivity as */
/* the filesystem. */
/* FileNameNCmp compares two CStr's (to length n) with the same */
/* case-sensitivity as the filesystem. */
/* FileNameIndCmp compares two PStr's (passing pointers to the string */
/* pointers) with the same case-sensitivity as the filesystem. */
#if Mac || MSWin || VxWorks
#define FileNameCmp		PStrCaseCmp
#define FileNameNCmp	StrNCaseCmp
#define FileNameIndCmp	PPStrCaseCmp
#elif Unix
#define FileNameCmp		PStrCmp
#define FileNameNCmp	StrNCmp
#define FileNameIndCmp	PPStrCmp
#endif

/** @brief Path type codes */
enum {	fAbsPath,
	fRelPath,
	fNotAPath,
	fUNCPath,		/**< uncfilename */
	nPathTypes };

TH_REENTRANT EXTERNC MgErr _FUNCC FRelPath(ConstPath start, ConstPath end, Path relPath);
TH_REENTRANT EXTERNC MgErr _FUNCC FAddPath(ConstPath basePath, ConstPath relPath, Path newPath);
TH_REENTRANT EXTERNC MgErr _FUNCC FAppendName(Path path, ConstPStr name);
TH_REENTRANT EXTERNC MgErr _FUNCC FName(ConstPath path, PStrHandle name);
TH_REENTRANT EXTERNC MgErr _FUNCC FNamePtr(ConstPath path, PStr name);
TH_REENTRANT EXTERNC MgErr _FUNCC FDirName(ConstPath path, Path dir);
TH_REENTRANT EXTERNC MgErr _FUNCC FVolName(ConstPath path, Path vol);
TH_REENTRANT EXTERNC Path _FUNCC FMakePath(Path path, int32 type, ...);
TH_REENTRANT EXTERNC Path _FUNCC FEmptyPath(Path);
TH_REENTRANT EXTERNC Path _FUNCC FNotAPath(Path);
TH_REENTRANT EXTERNC Bool32 _FUNCC FIsAPath(ConstPath);
TH_REENTRANT EXTERNC int32 _FUNCC FDepth(ConstPath path);

TH_REENTRANT EXTERNC MgErr _FUNCC FPathToPath(Path *p);
TH_REENTRANT EXTERNC MgErr _FUNCC FPathCpy(Path dst, ConstPath src);
TH_REENTRANT EXTERNC MgErr _FUNCC FTextToPath(ConstUPtr, int32, Path*);
TH_REENTRANT EXTERNC MgErr _FUNCC FPathToDSString(ConstPath, LStrHandle*);
TH_REENTRANT EXTERNC UHandle _FUNCC PathToCString(ConstPath );
TH_REENTRANT EXTERNC MgErr _FUNCC FStringToPath(ConstLStrH, Path*);
TH_REENTRANT EXTERNC MgErr _FUNCC FPathToFileSystemDSString(ConstPath p, LStrHandle *txt);
TH_REENTRANT EXTERNC MgErr _FUNCC FFileSystemCStrToPath(ConstCStr text, Path *p);

TH_REENTRANT EXTERNC Bool32 _FUNCC FStrFitsPat(const uChar*, const uChar*, int32, int32);
TH_REENTRANT EXTERNC int32 _FUNCC FPathCmp(ConstPath, ConstPath);
TH_REENTRANT EXTERNC int32 _FUNCC FPathCmpLexical(ConstPath, ConstPath);
TH_REENTRANT EXTERNC MgErr _FUNCC FSetPathType(Path, int32);
TH_REENTRANT EXTERNC MgErr _FUNCC FGetPathType(ConstPath, int32*);
TH_REENTRANT EXTERNC Bool32 _FUNCC FIsEmptyPath(ConstPath);
TH_REENTRANT EXTERNC Bool32 _FUNCC FIsAPathOrNotAPath(ConstPath);
TH_REENTRANT EXTERNC Bool32 _FUNCC FIsAPathOfType(ConstPath, int32);
TH_REENTRANT EXTERNC LStrHandle _FUNCC FGetDefGroup(LStrHandle);

/** @brief Releases memory and sets pointer to NULL. */
TH_REENTRANT EXTERNC void _FUNCC FDestroyPath(Path *pp);
/** @brief Use of FDestroyPath recommended over FDisposePath. */
TH_REENTRANT EXTERNC MgErr _FUNCC FDisposePath(Path p);

TH_REENTRANT EXTERNC int32 _FUNCC FUnFlattenPath(UPtr fp, Path *pPtr);
TH_REENTRANT EXTERNC int32 _FUNCC FFlattenPath(ConstPath p, UPtr fp);

/** @brief Get the absolute LV Path to directory where executable is. */
TH_REENTRANT EXTERNC MgErr _FUNCC FAppPath(Path p);

typedef MagicCookie LVRefNum;

TH_REENTRANT EXTERNC MgErr _FUNCC FNewRefNum(ConstPath, File, LVRefNum*);
TH_REENTRANT EXTERNC Bool32 _FUNCC FIsARefNum(LVRefNum);
TH_REENTRANT EXTERNC MgErr _FUNCC FDisposeRefNum(LVRefNum);
TH_REENTRANT EXTERNC MgErr _FUNCC FRefNumToFD(LVRefNum, File*);
TH_REENTRANT EXTERNC MgErr _FUNCC FRefNumToPath(LVRefNum, Path);
TH_REENTRANT EXTERNC MgErr _FUNCC FArrToPath(UHandle, Bool32, Path);
TH_REENTRANT EXTERNC MgErr _FUNCC FPathToArr(ConstPath, Bool32*, UHandle);

typedef MagicCookie LVUserEventRef;
TH_REENTRANT EXTERNC MgErr _FUNCC PostLVUserEvent(LVUserEventRef ref, void *data);

/** @brief Custom data used for external function calls (such as the Call Library Node). */
typedef void* InstanceDataPtr;

TH_REENTRANT EXTERNC MgErr _FUNCC CallChain(UHandle);

TH_REENTRANT EXTERNC MgErr _FUNCC NumericArrayResize(int32, int32, UHandle*, size_t);

/*** CIN-specific Functions ***/
TH_REENTRANT  EXTERNC int32 _FUNCC SetCINArraySize(uChar **elmtH, int32 elmtNum, int32 newNumElmts);
TH_REENTRANT  EXTERNC int32 _FUNCC GetDSStorage(void);
TH_REENTRANT  EXTERNC int32 _FUNCC SetDSStorage(int32 newval);
TH_REENTRANT  EXTERNC int16* _FUNCC GetTDPtr(void);
TH_REENTRANT  EXTERNC uChar* _FUNCC GetLVInternals(void);

/* selectors for CINProperties */
enum { kCINIsReentrant };

#define CIN EXTERNC

CIN int32 _FUNCC CINInit(void);
CIN int32 _FUNCC CINDispose(void);
CIN int32 _FUNCC CINAbort(void);
CIN int32 _FUNCC CINLoad(uInt32 reserved);
CIN int32 _FUNCC CINUnload(void);
CIN int32 _FUNCC CINSave(uInt32 reserved);
CIN int32 _FUNCC CINProperties(int32 selector, void *arg);

#define ENTERLVSB
#define LEAVELVSB

/* CINInit -- Called after the VI is loaded or recompiled. */
#define UseDefaultCINInit CIN MgErr _FUNCC CINInit() { return mgNoErr; }

/* CINDispose -- Called before the VI is unloaded or recompiled.*/
#define UseDefaultCINDispose CIN MgErr _FUNCC CINDispose() { return mgNoErr; }

/* CINAbort-- Called when the VI is aborted. */
#define UseDefaultCINAbort CIN MgErr _FUNCC CINAbort() { return mgNoErr; }

/* CINLoad -- Called when the VI is loaded. */
#define UseDefaultCINLoad CIN MgErr _FUNCC CINLoad(uInt32 reserved) { Unused(reserved); return mgNoErr; }

/* CINUnload -- Called when the VI is unloaded. */
#define UseDefaultCINUnload CIN MgErr _FUNCC CINUnload() { return mgNoErr; }

/* CINSave -- Called when the VI is saved. */
#define UseDefaultCINSave CIN MgErr _FUNCC CINSave(uInt32 reserved) { Unused(reserved); return mgNoErr; }

#include "lv_epilog.h"

#endif /* _extcode_H */
