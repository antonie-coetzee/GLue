#if !defined _BasicString_H_
#define _BasicString_H_

void* fmemcpy(void *dst0, const void *src0, size_t length);
void* fmemmove(void *s1, const void *s2, size_t n);
void *fmemset(void *dst, int c, size_t n);
size_t fstrlen(const char *str);
#endif