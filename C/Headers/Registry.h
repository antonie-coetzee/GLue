#if !defined _REGISTRY_H_
#define _REGISTRY_H_

#include "Glue.h"

typedef int (*GlueAction)(GLue*);

int AddToRegistry(GLue *glueRef);
int RemoveFromRegistry(GLue *glueRef);
int DisposeSingle(GLue* glue);

#endif
