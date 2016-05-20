#include <stdlib.h>
#include <stdio.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "extcode.h"
#include "Module.h"
#include "Util.h"
#include "Shared.h"
#include "PushValues.h"
#include "PullValues.h"
#include "BasicString.h"

GLUE_API int PushChunk(GLue **GLueRef,char *ChunkName, LStrHandle LVString)
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
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

GLUE_API int NewTable(GLue **GLueRef)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_newtable (glue->CoThread);	
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int CreateTable(GLue **GLueRef, int *narr, int *nrec)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_createtable(glue->CoThread, *narr, *nrec);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

	/* create NamedCluster */
GLUE_API int PushNamedCluster(GLue **GLueRef)
{
	GLue *glue = *GLueRef;
	lua_State *L;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	L = glue->CoThread;
	lua_newtable(L);
	CreateNamedClusterMetaTable(L);
	lua_setmetatable(L, -2);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int PushLVVariant(GLue **GLueRef, LStrHandle LVString)
{
	GLue *glue = *GLueRef;	
	LVUdata *UVariant;
	size_t nbytes;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	nbytes = sizeof(LVUdata) + (LStrLen(*LVString) - 1)*sizeof(uChar);
	UVariant = (LVUdata*) lua_newuserdata(glue->CoThread, nbytes);
	luaL_getmetatable(glue->CoThread, "variant");
	lua_setmetatable(glue->CoThread, -2);
	fmemcpy(UVariant->str, LStrBuf(*LVString),LStrLen(*LVString));
	UVariant->cnt = LStrLen(*LVString);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int PushLVClusterArray(GLue **GLueRef)
{
	GLue *glue = *GLueRef;	
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_newtable(glue->CoThread);
	luaL_getmetatable(glue->CoThread, "clusterArray");
	lua_setmetatable(glue->CoThread, -2);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int PushGlobal(GLue **GLueRef, char *Name)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_getglobal(glue->CoThread, Name);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int PushNil(GLue **GLueRef)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_pushnil(glue->CoThread);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int PushBool(GLue **GLueRef, int *LVBool)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_pushboolean (glue->CoThread, *LVBool);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int PushFloat(GLue **GLueRef, double *LVNumber)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));	
	lua_pushnumber(glue->CoThread, *LVNumber);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int PushString(GLue **GLueRef, LStrHandle LVString)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));	
	ERROR_CATCH(glue);
	lua_pushlstring(glue->CoThread,(const char*)LStrBuf(*LVString),LStrLen(*LVString));
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int SetTableNil(GLue **GLueRef, LStrHandle Name)
{
	GLue *glue = *GLueRef;	
	PushString(GLueRef, Name);
	PushNil(GLueRef);
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));	
	ERROR_CATCH(glue);
	lua_settable((*GLueRef)->CoThread, -3);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int SetTableBool(GLue **GLueRef, LStrHandle Name, int *LVBool)
{
	GLue *glue = *GLueRef;	
	PushString(GLueRef, Name);
	PushBool(GLueRef, LVBool);
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));	
	ERROR_CATCH(glue);
	lua_settable((*GLueRef)->CoThread, -3);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int SetTableNumber(GLue **GLueRef, LStrHandle Name, double *LVNumber)
{
	GLue *glue = *GLueRef;
	PushString(GLueRef, Name);
	PushFloat(GLueRef, LVNumber);
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_settable(glue->CoThread, -3);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int SetTableString(GLue **GLueRef, LStrHandle Name, LStrHandle String)
{
	GLue *glue = *GLueRef;
	PushString(GLueRef, Name);
	PushString(GLueRef, String);
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_settable(glue->CoThread, -3);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

int PushBoolElement(lua_State *l, void* lvArray, int index)
{
	lua_pushboolean(l, (*(LVBoolArray)lvArray)->Items[index]);
	return 0;
}

GLUE_API int PushBoolArray(GLue **GLueRef, LVBoolArray lvArray)
{
	GLue *glue = *GLueRef;
	int ret;
	int numrow = (*lvArray)->dimSizes[0];
	int numcol = (*lvArray)->dimSizes[1];
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	ret = PushArray(glue, "boolArray",numrow,numcol, PushBoolElement, lvArray);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

int PushDoubleElement(lua_State *l, void* lvArray, int index)
{
	lua_pushnumber(l, (*(LVDoubleArray)lvArray)->Items[index]);
	return 0;
}

GLUE_API int PushDoubleArray(GLue **GLueRef, LVDoubleArray lvArray)
{
	GLue *glue = *GLueRef;
	int ret;
	int numrow = (*lvArray)->dimSizes[0];
	int numcol = (*lvArray)->dimSizes[1];
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
    ret = PushArray(glue, "numberArray",numrow,numcol, PushDoubleElement, lvArray);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

int PushStringElement(lua_State *l, void* lvArray, int index)
{
	LStrHandle LVString = (*(LVStringArray)lvArray)->Items[index];
	lua_pushlstring(l,(const char*)LStrBuf(*LVString),LStrLen(*LVString));
	return 0;
}

GLUE_API int PushStringArray(GLue **GLueRef, LVStringArray lvArray)
{
	GLue *glue = *GLueRef;
	int ret;
	int numrow = (*lvArray)->dimSizes[0];
	int numcol = (*lvArray)->dimSizes[1];
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	ret = PushArray(glue, "stringArray",numrow,numcol, PushStringElement, lvArray);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}