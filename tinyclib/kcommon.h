#pragma once

#define	NELEMS(a)	(sizeof(a) / sizeof(a[0]))

typedef int (*fn_cmp)(void *, void *);
typedef unsigned (*fn_hash)(void *, unsigned);
int       _strcmp(void*, void*);

char* kprogname(void);
void* kmalloc(size_t n);
void kprintf(const char* fmt, ...);