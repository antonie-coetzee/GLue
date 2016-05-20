#if !defined _Shared_H_
#define _Shared_H_

#include <setjmp.h>
#include "Glue.h"
#include "extcode.h"
#include "lua.h"

#define GlueFromLuaState(L) ((GLue*)*(size_t*)((char*)L - sizeof(void*)))

typedef int (*ElementAction)(lua_State *L, void *lvArray, int index);

typedef  MgErr (__cdecl *tNumericArrayResize)(int32,int32,UHandle*,size_t);
typedef  MgErr (__cdecl *tMoveBlock)(const void*,void*,size_t);
typedef  MgErr (__cdecl *tPostLVUserEvent)(LVUserEventRef, void*);

typedef void (_cdecl *tlongjmp)(jmp_buf,int);
typedef int (_cdecl *tsetjmp)(jmp_buf);

typedef void* (_cdecl *tmalloc)(size_t);
typedef void (_cdecl *tfree)(void*);
typedef void* (_cdecl *trealloc)(void*,size_t);

extern tNumericArrayResize fNumericArrayResize;
extern tMoveBlock fMoveBlock;
extern tPostLVUserEvent fPostLVUserEvent;

extern tlongjmp flongjmp;
extern tsetjmp fsetjmp;
extern tmalloc fmalloc;
extern tfree ffree;
extern trealloc frealloc;

int CopyString2Labview(char* string,LStrHandle lvStringHandle, int len);
int AddMetaTable(lua_State *L, const char *ID, LVType type);
int PullLVString(lua_State *L, int SIndex, LStrHandle LVString);
void StackDump (lua_State *L, LStrHandle LVString);
int GetLVType(lua_State *L, int index);
int GetDimensions(lua_State *L, int *Rows, int *Columns, int crnttype, int *is2D);
int CreateNamedClusterMetaTable(lua_State *L);
void *tlsf_lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize);
void HookRoutine(lua_State *l, lua_Debug *ar);
int SetLuaPath( lua_State* L, const char* Lpaths,  const char* Cpaths);
int PushArray(GLue *glue, const char* type,int numrow, int numcol, ElementAction pusher, void* lvArray);
int PullArray(GLue *glue, int is2d, int numrow, int numcol, ElementAction action, void* lvArray);
void CloseGlue(GLue *GLueRef);
void CloseInterruptedGlue(GLue *GLueRef);

#endif