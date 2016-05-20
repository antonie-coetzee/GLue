#include <windows.h>
#include "Registry.h"
#include "queue.h"
#include "extcode.h"
#include "Glue.h"
#include "lua.h"
#include "Shared.h"
#include "BasicString.h"

extern void LinkLuaFunctions(void);

CRITICAL_SECTION regCritSec;
int isInitialized = 0;
int isReserved = 0;
long long created = 0;

LIST_HEAD(GlueList,GLue) glue_list;

void Initialize(void)
{
	if(isInitialized == 0)
	{
		isInitialized = 1;
		LIST_INIT(&glue_list);
		InitializeCriticalSection(&regCritSec);
		{
			HMODULE hlv;
			FARPROC proc;
			HINSTANCE crtLib;
			hlv = GetModuleHandle(NULL);
			proc = GetProcAddress(hlv,"NumericArrayResize");
			// null if we running in the LabVIEW Runtime, load LabVIEW runtime dll
			if(proc == NULL)
			{
				hlv = LoadLibrary(TEXT("lvrt.dll"));
				proc = GetProcAddress(hlv,"NumericArrayResize");
			}
			fNumericArrayResize = (tNumericArrayResize)proc;
			proc = GetProcAddress(hlv,"MoveBlock");
			fMoveBlock = (tMoveBlock)proc;
			proc = GetProcAddress(hlv,"PostLVUserEvent");
			fPostLVUserEvent = (tPostLVUserEvent)proc;

			crtLib = LoadLibrary(TEXT("msvcrt.dll"));
			fmalloc = (tmalloc)GetProcAddress(crtLib,"malloc");
			ffree = (tfree)GetProcAddress(crtLib,"free");
			frealloc = (trealloc)GetProcAddress(crtLib,"realloc");
			flongjmp = (tlongjmp)GetProcAddress(crtLib,"longjmp");
			fsetjmp = (tsetjmp)GetProcAddress(crtLib,"_setjmp");
			LinkLuaFunctions();
		}
	}
}

int AddToRegistry(GLue *glueRef)
{
	if(glueRef == NULL)
	{
		return -1;
	}
	EnterCriticalSection(&regCritSec);
	LIST_INSERT_HEAD(&glue_list,glueRef,GluePointers);
	LeaveCriticalSection(&regCritSec);
	return 0;
}

int RemoveFromRegistry(GLue *glueRef)
{
	EnterCriticalSection(&regCritSec);
	LIST_REMOVE(glueRef, GluePointers);
	LeaveCriticalSection(&regCritSec);
	return 0;
}

int DisposeAll(GlueAction disposer)
{
	EnterCriticalSection(&regCritSec);		
	while(!LIST_EMPTY(&glue_list))
	{
		GLue* glue = LIST_FIRST(&glue_list);
		LIST_REMOVE(glue, GluePointers);
		disposer(glue);
	}
	LeaveCriticalSection(&regCritSec);
	return 0;
}

int DisposeGlueInstance(GLue* glue)
{
	CRITICAL_SECTION *glue_section;
	glue->exit = 1;
	//lua_sethook(glue->CoThread, HookRoutine, LUA_MASKLINE | LUA_MASKCALL | LUA_MASKRET, 0);
	glue_section = &(glue->critSec);
	EnterCriticalSection(glue_section);	
	LeaveCriticalSection(glue_section);
	CloseGlue(glue);
	return 0;
}

int DisposeSingle(GLue *glueRef)
{
	EnterCriticalSection(&regCritSec);		
	LIST_REMOVE(glueRef, GluePointers);
	DisposeGlueInstance(glueRef);
	LeaveCriticalSection(&regCritSec);
	return 0;
}

GLUE_API MgErr ReserveRegistry(InstanceDataPtr *instancePtr)
{
	Initialize();
	EnterCriticalSection(&regCritSec);
	created++;	
	LeaveCriticalSection(&regCritSec);
	return 0;
}

GLUE_API MgErr AbortRegistry(InstanceDataPtr *instancePtr)
{
	GLue  *glue;
	EnterCriticalSection(&regCritSec);		
	LIST_FOREACH(glue,&glue_list,GluePointers)
	{
		glue->exit = 1;
		//lua_sethook(glue->CoThread, HookRoutine, LUA_MASKLINE | LUA_MASKCALL | LUA_MASKRET, 0);
	}
	LeaveCriticalSection(&regCritSec);
	return 0;
}

GLUE_API MgErr UnreserveRegistry(InstanceDataPtr *instancePtr)
{
	long long crntCnt = 0;
	EnterCriticalSection(&regCritSec);
		crntCnt = --created;
	LeaveCriticalSection(&regCritSec);
	if(crntCnt <= 0)
	{
		DisposeAll(DisposeGlueInstance);
	}	
	return 0;
}
