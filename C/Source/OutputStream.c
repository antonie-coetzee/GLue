#include "OutputStreamType.h"
#include "OutputStream.h"
#include "ringbuffer.h"
#include "Shared.h"
#include "glue.h"

int InitOutputStream(GLue *glue, int size)
{
	bufferInit(glue->OutputBuffer,size,char,fmalloc);
	return 0;
}

int FreeOutputStream(GLue *glue)
{
	CharRingBuffer* buffer;
	buffer = &(glue->OutputBuffer);
	bufferDestroy(buffer,ffree);
	return 0;
}

int ResizeOutputStream(GLue *glue, int size)
{
	EnterCriticalSection(&(glue->OutputStreamCritSec));
	bufferResize((glue->OutputBuffer),size,char,ffree,fmalloc);
	LeaveCriticalSection(&(glue->OutputStreamCritSec));
	return 0;
}

int AppendToOutputStream(GLue *glue, char* string, int size)
{
	int i;
	CharRingBuffer* buffer;
	buffer = &(glue->OutputBuffer);
	for(i = 0;i<size;i++)
	{
		bufferWrite(buffer,(string[i]));
	}	
	return 0;
}

int CurrentCountOutputStream(GLue *glue)
{
	int cnt = 0;
	CharRingBuffer* buffer;
	buffer = &(glue->OutputBuffer);
	bufferCount(buffer,cnt);
	return cnt;
}

int ClearOutputStream(GLue *glue)
{
	CharRingBuffer* buffer;
	buffer = &(glue->OutputBuffer);
	bufferClear(buffer);
	return 0;
}

char GetNextBufferValue(void* bufferRef)
{
	char elem;
	CharRingBuffer* buffer = (CharRingBuffer*)bufferRef;
	bufferRead(buffer,elem);
	return elem;
}

int ReadOutputStream(GLue *glue, BufferReader reader, void *userRef)
{
	int crntCnt;
	CharRingBuffer* buffer;
	buffer = &(glue->OutputBuffer);
	bufferCount(buffer,crntCnt);
	reader(buffer, crntCnt, GetNextBufferValue,userRef);
	return 0;
}