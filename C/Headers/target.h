#ifndef _TARGET_H_
#define _TARGET_H_

#include <windows.h>

#define TLSF_MLOCK_T            CRITICAL_SECTION
#define TLSF_CREATE_LOCK(l)     InitializeCriticalSection(l)
#define TLSF_DESTROY_LOCK(l)    DeleteCriticalSection(l)
#define TLSF_ACQUIRE_LOCK(l)    EnterCriticalSection(l)
#define TLSF_RELEASE_LOCK(l)    LeaveCriticalSection(l)

#endif
