#if !defined _MemAlloc_H_
#define _MemAlloc_H_

#include <windows.h>

void *basic_lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize); 
void *tlsf_lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize);
void TlsfGlueFree(void *gluePtr);
#endif