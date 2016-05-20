#if !defined _PushValues_H_
#define _PushValues_H_

#include "Glue.h"

GLUE_API int PushChunk(GLue **GLueRef,char *ChunkName, LStrHandle LVString);
GLUE_API int NewTable(GLue **GLueRef);
GLUE_API int CreateTable(GLue **GLueRef, int *narr, int *nrec);
GLUE_API int PushLVVariant(GLue **GLueRef, LStrHandle LVString);
	/* create NamedCluster */
GLUE_API int PushNamedCluster(GLue **GLueRef);
GLUE_API int PushGlobal(GLue **GLueRef, char *Name);
GLUE_API int PushBool(GLue **GLueRef, int *LVBool);
GLUE_API int PushFloat(GLue **GLueRef, double *LVNumber);
GLUE_API int PushString(GLue **GLueRef, LStrHandle LVString);
GLUE_API int PushNil(GLue **GLueRef);
GLUE_API int SetTableNil(GLue **GLueRef, LStrHandle Name);
GLUE_API int SetTableBool(GLue **GLueRef, LStrHandle Name, int *LVBool);
GLUE_API int SetTableNumber(GLue **GLueRef, LStrHandle Name, double *LVNumber);
GLUE_API int SetTableString(GLue **GLueRef, LStrHandle Name, LStrHandle String);
GLUE_API int PushDoubleArray(GLue **GLueRef, LVDoubleArray LVDoubleArray);
GLUE_API int PushStringArray(GLue **GLueRef, LVStringArray LVStringArray);
GLUE_API int PushBoolArray(GLue **GLueRef, LVBoolArray LVBoolArray);

#endif