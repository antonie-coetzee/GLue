#if !defined _GLue_H_
#define _GLue_H_

#include <windows.h>
#include <setjmp.h>
#include "queue.h"
#include "extcode.h"
#include "lua.h"
#include "OutputStreamType.h"

#define GLUE_API __declspec(dllexport)

#define CHECK_EXIT(x) do{if(x->exit != 0){return -1;} } while(0,0)
#define CHECK_EXIT_AGAIN(x) do{if(x->exit != 0){LeaveCriticalSection(&(x->critSec));return -1;} } while(0,0)
#define CHECK_EXIT_VOID(x) do{if(x->exit > 0){return NULL;} } while(0,0)

#define ERROR_CATCH(x) do{	\
	if(fsetjmp(x->ErrorCatchPoint)) \
	{ \
		LeaveCriticalSection(&(x->critSec)); \
		return -2; \
	} \
	}while(0,0) \

typedef void (_cdecl *gfree)(void*);

typedef struct GLue 
{
	CRITICAL_SECTION critSec;
	CRITICAL_SECTION OutputStreamCritSec;
	CharRingBuffer  OutputBuffer;
	LVUserEventRef*	OutputEvent;
	volatile int	exit;
	void*			MemPool;
	lua_State*		Lua;
	lua_State*		CoThread;
	int				ThreadIndex;
	int				FirstLuaAlloc;
	jmp_buf			ErrorCatchPoint;
	gfree			Free;
	LIST_ENTRY(GLue) GluePointers;
}GLue;

typedef enum 
{
	CLUSTERARRAY = 6,
	VARIANTWRAPPER = 5,
	STRINGARRAY = 4,
	DOUBLEARRAY = 3,
	BOOLARRAY = 2,
	UDATA = 1,
	NAMEDCLUSTER = -1,
} LVType;

typedef struct 
{
	int32	cnt;	
	uChar	str[1];		/* cnt bytes */
} LVUdata;

typedef struct {
	int32 dimSizes[2];
	LStrHandle Items[1];
	} LVSArray;
typedef LVSArray **LVStringArray;

typedef struct {
	int32 dimSizes[2];
	double Items[1];
	} LVDArray;
typedef LVDArray **LVDoubleArray;

typedef struct {
	int32 dimSizes[2];
	LVBoolean Items[1];
	} LVBArray;
typedef LVBArray **LVBoolArray;

GLUE_API int Init(GLue **GLueRef, LVUserEventRef *outputEvent, void* memPool, unsigned int memsize);
GLUE_API void Close(GLue **GLueRef);
GLUE_API int Debug(GLue **GLueRef, int *enable);
GLUE_API int RunString(GLue **GLueRef, char* Script, LStrHandle LVError);
GLUE_API int LoadBuffer(GLue **GLueRef,char *ChunkName, LStrHandle LVString);
GLUE_API int Resume(GLue **GLueRef, int *NumArg, LStrHandle LVError);
GLUE_API int ReInitThread(GLue **GLueRef);

#endif