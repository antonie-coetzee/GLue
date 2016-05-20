#include "MemoryAllocators.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "tlsf.h"

#include "Glue.h"
#include "Shared.h"
#include "BasicString.h"

void *tlsf_lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) 
{
	void *temp;
	GLue *glue = (GLue*)ud;

	if (nsize == 0) 
	{
		if(ptr == NULL)
		{
			return NULL;
		}
		if(ptr == glue->Lua)
		{
			void* truePtr = (char*)ptr - sizeof(void*);
			free_ex(truePtr,glue->MemPool);
			return NULL;
		}
		free_ex(ptr,glue->MemPool);
		return NULL;
	}
	else
	{
		if(glue->FirstLuaAlloc == 1)
		{
			void* temp;
			void* out;
			glue->FirstLuaAlloc = 0;
			temp = realloc_ex(ptr, sizeof(void*) + nsize,glue->MemPool);
			if(temp == NULL)
			{
				return NULL;
			}
			fmemcpy(temp,&glue,sizeof(void*));
			out = (char*)temp + sizeof(void*);
			glue->Lua = (lua_State*)out;
			return out;
		}
		else
		{
			return realloc_ex(ptr, sizeof(void*) + nsize,glue->MemPool);
		}	
		return temp; 
	}	 	  
}

void TlsfGlueFree(void *gluePtr)
{
	// do nothing.. no need to reclaim labview memory
	gluePtr = gluePtr;
}

void *basic_lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) 
{
	GLue *glue = (GLue*)ud;

	if (nsize == 0) 
	{
		if(ptr == NULL)
		{
			return NULL;
		}
		if(ptr == glue->Lua)
		{
			void* truePtr = (char*)ptr - sizeof(void*);
			ffree(truePtr);
			return NULL;
		}
		ffree(ptr);
		return NULL;
	}
	else
	{
		if(glue->FirstLuaAlloc == 1)
		{
			void* temp;
			void* out;
			glue->FirstLuaAlloc = 0;
			temp = frealloc(ptr, sizeof(void*) + nsize);
			if(temp == NULL)
			{
				return NULL;
			}
			fmemcpy(temp,&glue,sizeof(void*));
			out = (char*)temp + sizeof(void*);
			glue->Lua = (lua_State*)out;	
			return out;
		}
		else
		{
			return frealloc(ptr, sizeof(void*) + nsize);
		}		
	}	 	  
}