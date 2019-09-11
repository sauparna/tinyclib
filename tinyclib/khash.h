#pragma once
#include "kcommon.h"
typedef struct KHash KHash;

struct KHash {
	Node **tab;
	unsigned n, nel;
	fn_cmp cmp;
	fn_hash hash;
};

KHash      *newhash(unsigned, fn_cmp, fn_hash);
void      freehash(KHash *, void (*freedata)(void *));
Node      *hlookup(KHash *, Node *, int);
int       _strcmp(void *, void *);
unsigned  _strhash(void *, unsigned);
void      fprinthashstats(FILE *, KHash *);
void      fprinthash(FILE *, KHash *, void (*printdata)(void*, void*));
