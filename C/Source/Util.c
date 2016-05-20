#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "Glue.h"
#include "Util.h"
#include "Shared.h"
#include "OutputStream.h"
#include "PullValues.h"

	/* does a check if the table at index is a namedcluster, if so it returns the namelist size */
GLUE_API int CheckNamedCluster(GLue **GLueRef, int *size)
{
	lua_State *L;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	L = glue->CoThread;	
	if(luaL_getmetafield(L, -1, "__LVNC")){
			/* pop identifier */
		lua_pop(L,1);
		if(lua_getmetatable(L,-1)){
				/* get the length of the metatable */
			lua_len(L, -1);
			*size = (int)lua_tointeger(L,-1);
				/* pop length off the stack */
			lua_pop(L,1);
				/* pop metatable from stack */
			lua_pop(L,1);
		}else{
			*size = -1;
		}
	}else{
			/* not a namedcluster */
		*size = -1;
	}
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

	/* push onto stack the metatable of the value at index */
GLUE_API int GetMetatable(GLue **GLueRef, int *index)
{
	lua_State *L;
	int ret;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	L = glue->CoThread;
	ret = lua_getmetatable(L, *index);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

	/* get the Lua reference */
GLUE_API void* GetLuaRef(GLue **GLueRef)
{
	lua_State *L;
	GLue *glue = *GLueRef;
	CHECK_EXIT_VOID(glue);
	EnterCriticalSection(&(glue->critSec));
	L = glue->Lua;
	LeaveCriticalSection(&(glue->critSec));
	return L;
}

	/* set top value on stack as a global */
GLUE_API int SetGlobal(GLue **GLueRef, char *name)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_setglobal((*GLueRef)->CoThread, name);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

	/* retrieve the name list in correct order */
GLUE_API int GetNameList(GLue **GLueRef, int *index, int *length, LVStringArray LVStringArray)
{
	lua_State *L;
	int numrow, numcol, i;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	L = glue->CoThread;	
		/* dimSizes[0] is the number of rows */
	numrow = (*LVStringArray)->dimSizes[0];
		/* dimSizes[1] is the number of columns */
	numcol = (*LVStringArray)->dimSizes[1];
	if(lua_getmetatable(L, *index)){
			/* itterate through metaarray */
		for(i=0; i < *length; i++) {
			if(i < numcol){
				lua_rawgeti(L,-1,i+1);
				PullLVString(L,-1,(*LVStringArray)->Items[i]);
					/* remove 'value' from stack */
				lua_pop(L, 1);
			}			
		}
			/* pop metatable from stack */
		lua_pop(L, 1);
	}else{
		LeaveCriticalSection(&(glue->critSec));
		return -1;
	}
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int TableNext(GLue **GLueRef, int *index)
{
	int ret;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	ret = lua_next((*GLueRef)->CoThread, *index);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

GLUE_API int TableSet(GLue **GLueRef, int *index)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_settable((*GLueRef)->CoThread, *index);
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int TableGet(GLue **GLueRef, int *index)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	lua_gettable((*GLueRef)->CoThread, *index);	
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int TableGetRawI(GLue **GLueRef, int *index, int n)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	lua_rawgeti((*GLueRef)->CoThread, *index, n);	
	LeaveCriticalSection(&(glue->critSec));
	return 0;
}

GLUE_API int StackState(GLue **GLueRef, int *Size, int *TopType )
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	*Size = lua_gettop(glue->CoThread);
	*TopType = lua_type(glue->CoThread, -1);
	LeaveCriticalSection(&(glue->critSec));
	 return 0; 
}

GLUE_API int GetType(GLue **GLueRef, int *Index, int *Type)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	*Type = lua_type((*GLueRef)->CoThread, *Index);
	LeaveCriticalSection(&(glue->critSec));
	return 0; 
}

GLUE_API int GetStatus(GLue **GLueRef)
{
	int ret;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ret = lua_status(glue->CoThread);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}


GLUE_API int Pop(GLue **GLueRef, int *Num)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	lua_pop((*GLueRef)->CoThread, *Num);
	LeaveCriticalSection(&(glue->critSec));
	return 0; 
}

GLUE_API int SetTop(GLue **GLueRef, int *Index)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	lua_settop((*GLueRef)->CoThread, *Index);
	LeaveCriticalSection(&(glue->critSec));
	return 0; 
}

GLUE_API int GetStackTop(GLue **GLueRef)
{
	int ret;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ret = lua_gettop((*GLueRef)->CoThread);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

GLUE_API int Dump(GLue **GLueRef, LStrHandle outString)
{
	int top = 0;
	lua_State *L;
	L = (*GLueRef)->CoThread;
	StackDump(L, outString);
	top = lua_gettop(L);
	return top;
}

int ReadBuffer(void *bufferRef, int size, NextBufferValue getter, void* userRef)
{
	char* dest;
	int i;
	LStrHandle LVString = (LStrHandle)userRef;
	if(size > 0)
	{
		if(size > (*LVString)->cnt)
		{
			fNumericArrayResize(uB, 1L,(UHandle*)&LVString, size);
		}
		dest = (char*)(LStrBuf(*LVString));
		for(i = 0;i<size;i++)
		{
			dest[i] = getter(bufferRef);
		}
		LStrLen(*LVString)=size;
	}
	return 0;
}

GLUE_API int PullOutputBuffer(GLue **GLueRef, LStrHandle LVString)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->OutputStreamCritSec));
	ReadOutputStream(glue,ReadBuffer,LVString);
	LeaveCriticalSection(&(glue->OutputStreamCritSec));
	return 0; 
}

GLUE_API int GetLuaPaths(GLue **GLueRef, LStrHandle Lpaths,  LStrHandle Cpaths)
{
	lua_State *L;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	L = (*GLueRef)->CoThread;		
    lua_getglobal( L, "package");
	if(!lua_istable(L,-1)){
		lua_settop(L, 0);
		return -1;
	}
		/* first get the lua lib paths */
	lua_getfield(L,-1, "path");
		/* retrieve value */
	if(!PullLVString(L,-1,Lpaths)){
		return -1;
	}
		/* remove the lua lib path string from the stack */
	lua_pop(L, 1);
		/* get the lua Clib paths */
	lua_getfield(L,-1, "cpath");
		/* retrieve value */
	if(!PullLVString(L,-1,Cpaths)){
		return -1;
	}
		/* remove all from stack */
	lua_settop(L, 0);
	LeaveCriticalSection(&(glue->critSec));
    return 0; 
}

GLUE_API int SetPaths(GLue **GLueRef, char* Lpaths, char* Cpaths)
{
	int ret;
	GLue *glue = *GLueRef;
	lua_State *L;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	L = glue->CoThread;
	ret = SetLuaPath(L, Lpaths, Cpaths);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

GLUE_API int GetArraySizes(GLue **GLueRef, int *Rows, int *Columns, int *is2D)
{	
	int lvtype;
	lua_State *L;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	L = (*GLueRef)->CoThread;
	lvtype = GetLVType(L, -1);
		/* check that is valid for array */
	if(lvtype >= 2 && lvtype < 5 ){
			/* get dimensions of the array table */
		GetDimensions(L, Rows, Columns, lvtype, is2D);
	}
	LeaveCriticalSection(&(glue->critSec));
	return lvtype;
}

