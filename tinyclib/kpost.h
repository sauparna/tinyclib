#pragma once
typedef struct KPost KPost;

struct KPost {
	char *s;
	int f;
};

#define KPOST(x) (KPost*)(x)

KPost *newkpost(char*, int);
int  cmpkpost(void*, void*);
int  kpost_match_handler(void*, void*);
void freekpost(void*);
void fprintkpost(void*, void*);
void printkpost(void*, void*);
unsigned int hashkpost(void*, unsigned);
