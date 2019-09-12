#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kcommon.h"

static char* name = NULL; // program name for messages
static FILE* stream = NULL; // output stream for messages

int _strcmp(void* d1, void* d2)
{
	return strcmp((char*)d1, (char*)d2);
}

char* kprogname(void)
{
	return name;
}

void kprintf(const char* fmt, ...)
{
	va_list args;
	if (stream == NULL)
		stream = stderr;
	fflush(stdout);
	if (kprogname() != NULL)
		fprintf(stream, "%s: ", kprogname());
	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);
	if (fmt[0] != '\0' && fmt[strlen(fmt) - 1] == ':')
	{
		char buf[100];
		strerror_s(buf, 100, errno);
		fprintf(stream, " %s", buf);
	}
	fprintf(stream, "\n");
	exit(2); // conventional value for failed execution
}

void* kmalloc(size_t n)
{
	void* p;
	p = malloc(n);
	if (p == NULL)
		kprintf("malloc of %u bytes failed:", n);
	return p;
}