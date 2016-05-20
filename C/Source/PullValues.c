#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "extcode.h"

#include "Glue.h"
#include "Shared.h"
#include "Util.h"
#include "PullValues.h"

int __fltused=0;

GLUE_API int PullLVVariant(GLue **GLueRef,int *index, LStrHandle LVVariant)
{
	LVUdata *UVariant;
	void *ud;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ud = luaL_checkudata(glue->CoThread, *index, "variant");
	if(ud != NULL){
		UVariant = (LVUdata*)ud;
		fNumericArrayResize(uB, 1L,(UHandle*)&LVVariant, UVariant->cnt);
		fMoveBlock(UVariant->str, LStrBuf(*LVVariant), UVariant->cnt);
		LStrLen(*LVVariant)=UVariant->cnt;
		LeaveCriticalSection(&(glue->critSec));
		return 0;
	}
	LeaveCriticalSection(&(glue->critSec));
	return -1;
}


GLUE_API int PullBool(GLue **GLueRef, int *index, int *LVBool)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	if(lua_isboolean(glue->CoThread, *index))
	{
		*LVBool = lua_toboolean(glue->CoThread, *index);
		LeaveCriticalSection(&(glue->critSec));
		return 0;
	}
	LeaveCriticalSection(&(glue->critSec));
	return -1;
}

GLUE_API int PullNumber(GLue **GLueRef, int *index, double *Number)
{
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	if(lua_isnumber(glue->CoThread, *index))
	{
		*Number = lua_tonumber(glue->CoThread, *index);
		LeaveCriticalSection(&(glue->critSec));
		return 0;
	}
	LeaveCriticalSection(&(glue->critSec));
	return -1;
}

GLUE_API int PullString(GLue **GLueRef, int *index, LStrHandle LVString)
{
	GLue *glue = *GLueRef;
	int ret;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));	
	ERROR_CATCH(glue);
	ret = PullLVString((*GLueRef)->CoThread, *index, LVString);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
	 
}

int PullBoolElement(lua_State *l, void * lvArray, int index)
{
	(*(LVBoolArray)lvArray)->Items[index] = (LVBoolean)lua_toboolean(l,-1);
	return 0;
}

GLUE_API int PullBoolArray(GLue **GLueRef,int is2d, LVBoolArray lvArray)
{
	int ret = -1;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ret = PullArray(glue,is2d,(*lvArray)->dimSizes[0], (*lvArray)->dimSizes[1], PullBoolElement, lvArray);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

int PullDoubleElement(lua_State *l, void * lvArray, int index)
{
	(*(LVDoubleArray)lvArray)->Items[index] = lua_tonumber(l,-1);
	return 0;
}

GLUE_API int PullDoubleArray(GLue **GLueRef,int is2d, LVDoubleArray lvArray)
{
	int ret = -1;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ret = PullArray(glue,is2d,(*lvArray)->dimSizes[0], (*lvArray)->dimSizes[1], PullDoubleElement, lvArray);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}

int PullStringElement(lua_State *l, void * lvArray, int index)
{
	return PullLVString(l,-1,(*(LVStringArray)lvArray)->Items[index]);
}

GLUE_API int PullStringArray(GLue **GLueRef,int is2d, LVStringArray lvArray)
{
	int ret = -1;
	GLue *glue = *GLueRef;
	CHECK_EXIT(glue);
	EnterCriticalSection(&(glue->critSec));
	ERROR_CATCH(glue);
	ret = PullArray(glue,is2d,(*lvArray)->dimSizes[0], (*lvArray)->dimSizes[1], PullStringElement, lvArray);
	LeaveCriticalSection(&(glue->critSec));
	return ret;
}