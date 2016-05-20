#if !defined _PullValues_H_
#define _PullValues_H_

#include "Glue.h"

GLUE_API int PullLVVariant(GLue **GLueRef,int *index, LStrHandle LVVariant);
GLUE_API int PullBool(GLue **GLueRef, int *index, int *LVBool);
GLUE_API int PullNumber(GLue **GLueRef, int *index, double *Number);
GLUE_API int PullString(GLue **GLueRef, int *index, LStrHandle LVString);
GLUE_API int PullDoubleArray(GLue **GLueRef,int is2d, LVDoubleArray LVDoubleArray);
GLUE_API int PullStringArray(GLue **GLueRef,int is2d, LVStringArray LVStringArray);
GLUE_API int PullBoolArray(GLue **GLueRef,int is2d, LVBoolArray LVBoolArray);

#endif