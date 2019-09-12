#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kcommon.h"
#include "kpost.h"

KPost *newkpost(char *s, int f)
{
	KPost *t;
	t = (KPost *)kmalloc(sizeof(KPost));
	t->s = _strdup(s);
	t->f = f;
	return t;
}

int cmpkpost(void *t, void *t1)
{
	return strcmp(((KPost *)t)->s, ((KPost *)t1)->s);
}

int  kpost_match_handler(void *t, void *t1)
{
	((KPost *)t)->f += ((KPost *)t1)->f;
	return 1;
}

void freekpost(void *t)
{
	free(t);
}

void fprintkpost(void *stream, void *d)
{
	fprintf((FILE *)stream, "{%s:%d}", ((KPost *)d)->s, ((KPost *)d)->f);
}

void printkpost(void *fmt, void *d)
{
	printf((const char *)fmt, ((KPost *)d)->s, ((KPost *)d)->f);
}

unsigned hashkpost(void *data, unsigned size)
{
	static unsigned MULTIPLIER = 31;
	unsigned h;
	h = 0;
	for (char *p = ((KPost *)data)->s; *p != '\0'; p++)
		h = MULTIPLIER * h + *p;
	return h % size;
}
