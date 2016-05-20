#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "Util.h"
#include "Shared.h"
#include "Module.h"
#include "Glue.h"
#include "OutputStream.h"

static int newOutputData = 1;

int ReturnFromLabVIEW(lua_State *L)
{
	if(lua_toboolean(L,1))
		{
			return luaL_error(L,lua_tostring(L,2));
		}
	lua_remove(L, 1);
	return lua_gettop(L);
}

char* NumToString(int i, char b[])
{
	int shifter = i;
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

int BundleArguments(lua_State *L, int num, int start)
{
	int i = 0;
	lua_createtable(L,num,0); // create the container table
	CreateNamedClusterMetaTable(L);
	lua_setmetatable(L, -2);		
	for(i=0;i<num;i++)
	{
		char key[12] = {0};
		lua_pushnil(L);
		lua_copy(L,start,-1);
		lua_remove(L,start);
		lua_pushstring(L,NumToString(i + 1, key));
		lua_insert(L,-2);
		lua_settable(L,-3);
	}
	return 1;
}

int BundleArrayArguments(lua_State *L, int num)
{
	int i = 0;
	lua_createtable(L,num,0); // create the container table
	for(i=0;i<num;i++)
	{
		lua_insert(L,-2);
		lua_rawseti(L,-2,num-i);
	}
	return 1;
}

int WriteStream(lua_State *L)
{
	const char *inString;
	int replace = 0;
	int prevBufCount = 0;
	GLue *glue = GlueFromLuaState(L);		
	size_t inStringLen;

	inString = luaL_checklstring(L,1,&inStringLen);
	if(lua_gettop(L) >= 2)
	{
		if(lua_isboolean(L,2))
		{
			replace = lua_toboolean(L,2);
		}
		else
		{
			return luaL_error(L,"Second argument must be boolean type");
		}
	}

	EnterCriticalSection(&(glue->OutputStreamCritSec));
	prevBufCount = CurrentCountOutputStream(glue);
	if(replace == 1)
	{
		ClearOutputStream(glue);
	}
	if(inStringLen > 0)
	{	
		AppendToOutputStream(glue,(char*)inString,inStringLen);
		// only fire event if buffer was empty previously
		if(prevBufCount == 0)
		{
			// 1: normal event, 2: shutsdown the LV eventloop
			// from within labview
			fPostLVUserEvent(*(glue->OutputEvent),(void*)&newOutputData);
		}
	}
	LeaveCriticalSection(&(glue->OutputStreamCritSec));
	return 0;
}

int OutStreamSize(lua_State *L)
{
	int newSize;
	GLue *glue = GlueFromLuaState(L);	
	newSize = luaL_checkint(L,1);
	if(newSize <= 0)
	{
		return luaL_error(L, "Output buffer size cannot be zero or negative");
	}
	EnterCriticalSection(&(glue->OutputStreamCritSec));
	ResizeOutputStream(glue,newSize);
	LeaveCriticalSection(&(glue->OutputStreamCritSec));
	return 0;
}

int CallLabVIEW(lua_State *L)
{
	int Args = lua_gettop(L);
	switch(Args)
	{
		case 0:
		case 1:
			return luaL_error(L, "Require atleast module reference and command as arguments");
		case 2:
			if(luaL_checkudata(L, 1, "variant") == NULL)
				return luaL_error(L, "Module reference required");
			if(!lua_isstring(L,2))
				return luaL_error(L, "Command must be a string value");
			break;
		case 3:
			if(luaL_checkudata(L, 1, "variant") == NULL)
				return luaL_error(L, "Module reference required");
			if(!lua_isstring(L,2))
				return luaL_error(L, "Command must be a string value");
			break;
		default:
			if(luaL_checkudata(L, 1, "variant") == NULL)
				return luaL_error(L, "Module reference required");
			if(!lua_isstring(L,2))
				return luaL_error(L, "Command must be a string value");
			BundleArguments(L, Args - 2, 3);
			break;
	}
	return lua_yieldk(L,lua_gettop(L),0,ReturnFromLabVIEW);
}

int NewNamedCluster(lua_State *L)
{
	int numArgs = lua_gettop(L);
	 
	if(numArgs == 0)
	{
		lua_newtable(L);
		CreateNamedClusterMetaTable(L);
		lua_setmetatable(L, -2);
	}
	else
	{
		BundleArguments(L,numArgs,1);
	}
	return 1;
}

static int NewVariantWrapper(lua_State *L)
{
	int numArgs = lua_gettop(L);
	if(numArgs == 0)
	{
		return luaL_error(L, "Arguments missing");	
	}
	else if(numArgs == 1)
	{
		lua_newtable(L);
		luaL_getmetatable(L, "variantWrapper");
		lua_setmetatable(L, -2);
		lua_insert(L,-2);
		lua_rawseti(L, -2, 1);
		return 1;
	}
	else
	{
		BundleArguments(L, numArgs,1);
		lua_newtable(L);
		luaL_getmetatable(L, "variantWrapper");
		lua_setmetatable(L, -2);
		lua_insert(L,-2);
		lua_rawseti(L, -2, 1);
		return 1;
	}
}

int NewLVArray(lua_State *L, const char* tag)
{
	int numArgs = lua_gettop(L);
	if(numArgs == 0)
	{
		lua_newtable(L);
		luaL_getmetatable(L, tag);
		lua_setmetatable(L, -2);
		return 1;
	}
	else
	{
		BundleArrayArguments(L,numArgs);
		luaL_getmetatable(L, tag);
		lua_setmetatable(L, -2);
		return 1;
	}
}

int NewLVClusterArray(lua_State *L)
{
	return NewLVArray(L,"clusterArray");
}

int NewLVStringArray(lua_State *L)
{
	return NewLVArray(L,"stringArray");
}

int NewLVFloatArray(lua_State *L)
{
	return NewLVArray(L,"numberArray");
}

int NewLVBoolArray(lua_State *L)
{
	return NewLVArray(L,"boolArray");
}

int LoadModuleFromFile(lua_State *L)
{
	switch(lua_gettop(L))
	{
		case 0:
			return luaL_error(L, "Arguments missing");
		case 1:
			if(!lua_isstring(L,-1))
			{
				return luaL_error(L, "Path argument must be a string");
			}
			lua_pushboolean(L, 0);
			lua_pushliteral(L, "__Load");
			lua_insert(L, 1);
			lua_getglobal(L, "__GLue");
			lua_insert(L, 1);			
			return CallLabVIEW(L);
		case 2:
			if(!lua_isboolean(L,-1))
			{
				return luaL_error(L, "Reentrant argument must be a bool");
			}
			if(!lua_isstring(L,-2))
			{
				return luaL_error(L, "Path argument must be a string");
			}
			lua_pushliteral(L, "__Load");
			lua_insert(L, 1);
			lua_getglobal(L, "__GLue");
			lua_insert(L, 1);			
			return CallLabVIEW(L);
		default:
			return luaL_error(L, "Too many arguments");
	}
}

int FromVariant(lua_State *L)
{
	int numArgs = lua_gettop(L);

	if(numArgs != 1){return luaL_error(L, "Single argument required");}

	lua_pushliteral(L, "__FromVariant");
	lua_insert(L, 1);
	lua_getglobal(L, "__GLue");
	lua_insert(L, 1);
	return CallLabVIEW(L);
}

int LuaOpen_LVlib(lua_State *L)
 {
	struct luaL_Reg LVlib[] = {	  
	  {"stringArray", NewLVStringArray},
	  {"boolArray", NewLVBoolArray},
	  {"numberArray", NewLVFloatArray},
	  {"clusterArray", NewLVClusterArray},
	  {"cluster", NewNamedCluster},
	  {"toVariant", NewVariantWrapper},
	  {"fromVariant", FromVariant},
	  {"invokeModule", CallLabVIEW},
	  {"loadModule", LoadModuleFromFile},
	  {"writeStream", WriteStream},
	  {"streamSize", OutStreamSize},
	  {NULL, NULL}
	};
  luaL_newlib(L,LVlib);
  return 1;
 }


