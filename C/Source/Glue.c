#include <windows.h>
#include <setjmp.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "tlsf.h"

#include "extcode.h"
#include "Glue.h"
#include "PushValues.h"
#include "PullValues.h"
#include "Module.h"
#include "Shared.h"
#include "Util.h"
#include "Registry.h"
#include "BasicString.h"
#include "MemoryAllocators.h"
#include "OutputStream.h"

const int _fltused = 0;

BOOL _DllMainCRTStartup(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	return TRUE;
}

int LuaErrorCatcher(lua_State *L)
{
	GLue *glue = GlueFromLuaState(L);
	flongjmp(glue->ErrorCatchPoint,1);
	return 0;
}

GLUE_API int Init(GLue **GLueRef, LVUserEventRef *outputEvent, void* memPool, unsigned int memsize)
{
	int err = 0;
	lua_State *L;
	GLue *glue;

	if(memPool == NULL || memsize <= 0)
	{
		// use ms memory allocator
		glue = (GLue*)fmalloc(sizeof(GLue));
		*GLueRef = glue;
		if(glue == NULL)
		{
			return -2;
		}
		glue->FirstLuaAlloc = 1;
		L = lua_newstate(basic_lua_alloc, glue);
		if(L == NULL)
		{
			ffree(glue);
			return -2;
		}
		glue->Free = ffree;
	}
	else
	{		
		// use tlsf memory allocator
		init_memory_pool(memsize, memPool);
		glue = (GLue*)malloc_ex(sizeof(GLue), memPool);
		*GLueRef = glue;
		if(glue == NULL)
		{
			return -2;
		}
		glue->MemPool = memPool;
		glue->FirstLuaAlloc = 1;
		L = lua_newstate(tlsf_lua_alloc, glue);
		if(L == NULL)
		{
			return -2;
		}
		glue->Free = TlsfGlueFree;
	}	
			
	InitializeCriticalSection(&(glue->critSec));
	InitializeCriticalSection(&(glue->OutputStreamCritSec));
	InitOutputStream(glue, 1024);
	glue->OutputEvent = outputEvent;
	lua_atpanic(L,LuaErrorCatcher);
	if(fsetjmp(glue->ErrorCatchPoint))
	{
		if(memPool == NULL || memsize <= 0)
		{
			// ms allocator.. need to reclaim memory
			lua_close(glue->Lua);
			DeleteCriticalSection(&(glue->critSec));
			ffree(glue);
			return -2;
		}
		else
		{
			// tlsf, pool provided by labview.. no need to reclaim
			DeleteCriticalSection(&(glue->critSec));
			return -2;
		}		
	}
	glue->exit = 0;
	luaL_openlibs(L);
	luaL_requiref(L,"glue",LuaOpen_LVlib,0);
	lua_pop(L, 1);
	AddMetaTable(L, "clusterArray", CLUSTERARRAY);
	AddMetaTable(L, "numberArray", DOUBLEARRAY);
	AddMetaTable(L, "variant", UDATA);
	AddMetaTable(L, "variantWrapper", VARIANTWRAPPER);
	AddMetaTable(L, "stringArray", STRINGARRAY);
	AddMetaTable(L, "boolArray", BOOLARRAY);
	AddToRegistry(glue);
	//glue->CoThread = lua_newthread(L);
	glue->CoThread = L;
	glue->ThreadIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	return 0;
}

GLUE_API int ReInitThread(GLue **GLueRef)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	luaL_unref(glue->Lua, LUA_REGISTRYINDEX,glue->ThreadIndex);
	glue->CoThread = lua_newthread(glue->Lua);
	glue->ThreadIndex = luaL_ref(glue->Lua,LUA_REGISTRYINDEX);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API void Close(GLue **GLueRef)
{
	GLue *glue = *GLueRef;	
	DisposeSingle(glue);
}

GLUE_API int RunString(GLue **GLueRef, char* Script, LStrHandle LVError)
{
	GLue *glue = *GLueRef;
	int result = 0;
	lua_State *L;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	L = glue->CoThread;	
	result = luaL_dostring(L,Script);
	if(result != 0)
	{
		if(lua_isstring(L,-1))
		{
			PullLVString(L,-1,LVError);
			lua_pop(L, 1);
		}
	}
	LeaveCriticalSection(&(glue->critSec));
	return result;
}

GLUE_API int LoadBuffer(GLue **GLueRef,char *ChunkName, LStrHandle LVString)
{
	GLue *glue = *GLueRef;
	lua_State *L;
	int ret;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	L = glue->CoThread;
	ret = luaL_loadbuffer(L,(const char*)LStrBuf(*LVString),LStrLen(*LVString),ChunkName);
	if(ret != 0)
	{
		if(lua_isstring(L,-1))
			PullLVString(L,-1,LVString);
	}
	else
	{
	    lua_setglobal(L,ChunkName);
	}
	lua_settop(L, 0);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

GLUE_API int Resume(GLue **GLueRef, int *NumArg, LStrHandle LVError)
{
	GLue *glue = *GLueRef;
	lua_State *L;
	int ret;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	L = glue->CoThread;	
	ret = lua_resume(L,NULL,*NumArg);	
	if(!((ret == 0) || (ret == 1)) ){
		if(lua_isstring(L,-1))
		{
			PullLVString(L,-1,LVError);
			lua_settop(L, 0);
		}
	}
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}


