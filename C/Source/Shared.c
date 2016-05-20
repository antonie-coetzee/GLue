#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "Glue.h"
#include "Util.h"
#include "OutputStream.h"
#include "Shared.h"
#include "BasicString.h"

tNumericArrayResize fNumericArrayResize;
tMoveBlock fMoveBlock;
tPostLVUserEvent fPostLVUserEvent;
tlongjmp flongjmp;
tsetjmp fsetjmp;
tmalloc fmalloc;
tfree ffree;
trealloc frealloc;

void CloseGlue(GLue *GLueRef)
{
	lua_close(GLueRef->Lua);
	DeleteCriticalSection(&(GLueRef->critSec));
	DeleteCriticalSection(&(GLueRef->OutputStreamCritSec));
	FreeOutputStream(GLueRef);
	GLueRef->Free(GLueRef);
}

void CloseInterruptedGlue(GLue *GLueRef)
{
	lua_close(GLueRef->Lua);
	GLueRef->Free(GLueRef);
}

int SetLuaPath( lua_State* L, const char* Lpaths,  const char* Cpaths)
{
    lua_getglobal( L, "package");
	if(!lua_istable(L,-1)){
		lua_settop(L, 0);
		return 0;
	}
	if(Lpaths != NULL){
		lua_pushstring(L, Lpaths); 
		lua_setfield(L, -2, "path");
	}
	if(Cpaths != NULL){
		lua_pushstring(L, Cpaths); 
		lua_setfield(L, -2, "cpath"); 
	}
	lua_pop(L, 1); 
    return 0; 
}

void HookRoutine(lua_State *l, lua_Debug *ar)
{
	ar = ar;
	luaL_error(l,"exit");	
}


	/* new index metamethod for named clusters */
int NamedClusterNewIndex(lua_State *L)
{
	int size;
		/* get the named cluster metatable */
	if(lua_getmetatable(L, -3)){
		/* get the length of the metatable */
		lua_len(L, -1);
		size = (int)lua_tointeger(L,-1);
			/* pop length off the stack */
		lua_pop(L,1);
			/* push a copy of the original key */
		lua_pushvalue(L,-3);
			/* add key to metatable name list */
		lua_rawseti(L,-2,size + 1);
			/* pop metatable */
		lua_pop(L,1);
	}
		/* insert value into cluster table */
	lua_rawset(L, -3);
	return 0;
}

	/* add a metatable for named clusters */
int CreateNamedClusterMetaTable(lua_State *L)
{
		/* new table to use as metatable */
	lua_newtable(L);
		/* identifier */
	lua_pushliteral(L, "__LVNC");
	lua_pushliteral(L, "");
	lua_settable(L,-3);
		/* new index meta method */
	lua_pushliteral(L, "__newindex");
	lua_pushcfunction(L, NamedClusterNewIndex);
	lua_settable(L,-3);
	//	/* protect metatable */
	lua_pushliteral(L, "__metatable");
	lua_pushliteral(L, "cluster");
	lua_settable(L,-3);
	lua_pushliteral(L,"__LVType");
	lua_pushinteger(L, NAMEDCLUSTER);
	lua_rawset(L,-3);
	return 0;
}

int CopyString2Labview(char* string,LStrHandle lvStringHandle, int len)
{
	fNumericArrayResize(uB, 1L,(UHandle*)&lvStringHandle, len);
	fMoveBlock(string, LStrBuf(*lvStringHandle), len);
	LStrLen(*lvStringHandle)=len;
	return len;
}

int AddMetaTable(lua_State *L, const char *ID, LVType type)
{
	luaL_newmetatable(L, ID);
	lua_pushliteral(L, "__metatable");
	lua_pushlstring(L, ID, fstrlen(ID));
	lua_settable(L,-3);
	lua_pushliteral(L,"__LVType");
	lua_pushinteger(L, type);
	lua_rawset(L,-3);
	lua_settop(L, 0);
	return 0;
}

	/* get the lv type of the table */
int GetLVType(lua_State *L, int index)
{
	int lvtype;
		/* push metable if there is one */
	if(lua_getmetatable(L, index))
	{
		lua_pushliteral(L,"__LVType");
		lua_rawget(L,-2);
		lvtype = lua_tointeger(L, -1);
		lua_pop(L,2);
		return (int)lvtype;
	}
	else
	{
		return -1;
	}
}

int GetDimensions(lua_State *L, int *Rows, int *Columns, int crnttype, int *is2D)
{
	*Rows =  (int)lua_rawlen(L, -1);
	lua_rawgeti(L,-1,1);
	if(lua_type(L,-1) == LUA_TTABLE){
		if(crnttype == GetLVType(L, -1)){
			*Columns =  (int)lua_rawlen(L, -1);
			*is2D = 1;
		}else{
			*Columns = 0;
			*is2D = 0;
		}
	}
	lua_pop(L,1);
	return 0;
}

void StackDump (lua_State *L, LStrHandle LVString) 
{
	lua_pushliteral(L, "__LVNC");
	PullLVString(L,-1,LVString);
	lua_pop(L, 1);
}

	/* retrieve lua paths */
int getLuaPath(GLue **GLueRef, LStrHandle Lpaths,  LStrHandle Cpaths)
{
	lua_State *L;
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
    return 0; 
}

int PullLVString(lua_State *L, int SIndex, LStrHandle LVString)
{
	const char* Temp = NULL; 
	size_t Length = 0;

	if(lua_isstring(L,SIndex))
	{
		Temp = lua_tolstring(L, SIndex, &Length);
	}
	else if(lua_isboolean(L,SIndex))
	{
		if(lua_toboolean(L,SIndex))
		{
			Temp = "true";
			Length = 4;
		}
		else
		{
			Temp = "false";
			Length = 5;
		}
	}
	else
	{
		Temp = "";
		Length = 0;
	}
	if(Temp != NULL)
	{
		fNumericArrayResize(uB, 1L,(UHandle*)&LVString, Length);
		fMoveBlock(Temp, LStrBuf(*LVString), Length);
		LStrLen(*LVString)=(int32)Length;
		return 1;
	}
	else
	{
		return 0;
	}
}

int PushArray(GLue *glue, const char* type,int numrow, int numcol, ElementAction pusher, void* lvArray)
{
	lua_State *LuaThread = glue->CoThread;
	int i, j;
	switch(numrow){
		case 0:
			lua_createtable(LuaThread,0,0);
			luaL_getmetatable(LuaThread, type);
			lua_setmetatable(LuaThread, -2);
			break;
		case 1:
			lua_createtable(LuaThread,numcol,0);
			luaL_getmetatable(LuaThread, type);
			lua_setmetatable(LuaThread, -2);
				for(j = 0; j < numcol; j++)
				{
					pusher(LuaThread,lvArray,j);
					lua_rawseti(LuaThread, -2, j + 1);
				}
			break;
		default:
			lua_createtable(LuaThread,numrow,0);
			luaL_getmetatable(LuaThread, type);
			lua_setmetatable(LuaThread, -2);
			for(i = 0; i < numrow; i++){
				lua_createtable(LuaThread,numcol,0);
				luaL_getmetatable(LuaThread, type);
				lua_setmetatable(LuaThread, -2);
				for(j = 0; j < numcol; j++)
				{
					pusher(LuaThread,lvArray,(i * numcol) + j);
					lua_rawseti(LuaThread, -2, j + 1);
				}
				lua_rawseti(LuaThread, -2, i + 1);
			}
			break;
	}
	return 0;
}

int PullArray(GLue *glue,int is2d, int numrow, int numcol, ElementAction action, void* lvArray)
{
	int i, j;
	lua_State *LuaThread = glue->CoThread;
	
	if(is2d == 0)
	{
		for(j = 0; j < numcol; j++)
		{
			lua_rawgeti(LuaThread,-1,j + 1);
			action(LuaThread,lvArray,j);
			lua_pop(LuaThread,1);
		}
	}
	else if (is2d == 1)
	{
		for(i = 0; i < numrow; i++)
		{
			lua_rawgeti(LuaThread,-1, i + 1);
			if(!lua_istable(LuaThread,-1))
			{
				lua_pop(LuaThread,1);
				continue;
			}
			for(j = 0; j < numcol; j++)
			{
				lua_rawgeti(LuaThread,-1,j + 1);
				action(LuaThread,lvArray,(i * numcol) + j);
				lua_pop(LuaThread,1);
			}
			lua_pop(LuaThread,1);
		}
	}
	return 0;
}



