#if !defined _Util_H_
#define _Util_H_

#include "Glue.h"

GLUE_API int GetLuaPath(GLue **GLueRef, LStrHandle Lpaths,  LStrHandle Cpaths);
GLUE_API int GetStackTop(GLue **GLueRef);
	/* does a check if the table at index is a namedcluster, if so it returns the namelist size */
GLUE_API int CheckNamedCluster(GLue **GLueRef, int *size);
	/* push onto stack the metatable of the value at index */
GLUE_API int GetMetatable(GLue **GLueRef, int *index);
	/* get the Lua reference */
GLUE_API void* GetLuaRef(GLue **GLueRef);
	/* set top value on stack as a global */
GLUE_API int SetGlobal(GLue **GLueRef, char *name);
	/* retrieve the name list in correct order */
GLUE_API int GetNameList(GLue **GLueRef, int *index, int *length, LVStringArray LVStringArray);
GLUE_API int GetType(GLue **GLueRef, int *Index, int *Type);
GLUE_API int GetStatus(GLue **GLueRef); 
GLUE_API int Pop(GLue **GLueRef, int *Num);
GLUE_API int SetTop(GLue **GLueRef, int *Index);
GLUE_API int StackState(GLue **GLueRef, int *Top, int *TopType);
GLUE_API int TableNext(GLue **GLueRef, int *index);
GLUE_API int TableGet(GLue **GLueRef, int *index);
	/* get from table using lua_rawgeti */
GLUE_API int TableGetRawI(GLue **GLueRef, int *index, int n);
GLUE_API int CurrentLine(GLue **GLueRef, int *Write, int *Value);
GLUE_API int GetArraySizes(GLue **GLueRef, int *Rows, int *Columns, int *is2D);
GLUE_API int TableSet(GLue **GLueRef, int *index);
GLUE_API int SetPaths(GLue **GLueRef, char* Lpaths, char* Cpaths);
GLUE_API int GetLuaPaths(GLue **GLueRef, LStrHandle Lpaths,  LStrHandle Cpaths);
GLUE_API int Dump(GLue **GLueRef, LStrHandle StackDump);
GLUE_API int PullOutputBuffer(GLue **GLueRef, LStrHandle LVString);
#endif