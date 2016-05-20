#if !defined _OUTPUTBUFFER_H_
#define _OUTPUTBUFFER_H_
#include <stdlib.h>
#include <stdio.h>
#include "ringbuffer.h"
#include "OutputStreamType.h"
#include "Glue.h"

typedef char (*NextBufferValue)(void *bufferRef);
typedef int (*BufferReader)(void *bufferRef, int size, NextBufferValue getter, void* userRef);

int InitOutputStream(GLue *glue, int size);

int FreeOutputStream(GLue *glue);
int ResizeOutputStream(GLue *glue, int size);
int AppendToOutputStream(GLue *glue, char* string, int size);
int ClearOutputStream(GLue *glue);
int ReadOutputStream(GLue *glue, BufferReader reader, void *userRef);
int CurrentCountOutputStream(GLue *glue);

#endif