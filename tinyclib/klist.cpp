#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "klist.h"
#include "kcommon.h"

Node *newnode(void *data)
{
	Node *n = (Node *)kmalloc(sizeof(Node));
	n->data = data;
	n->next = NULL;
	return n;
}

void freenode(Node *n, void (*freedata)(void*))
{
	if (freedata)
		(*freedata)(n->data);
	free(n);
}

void freelist(Node *n, void (*freedata)(void*))
{
	Node *next;
	for (; n != NULL; n = next) {
		next = n->next;
		if (freedata != NULL)
			(*freedata)(n->data);
		free(n);
	}
}

Node *addfront(Node *p, Node *n)
{
	n->next = p;
	return n;
}

Node *addend(Node *p, Node *n)
{
	Node *i;
	if (p == NULL)
		return n;
	for (i = p; i->next != NULL; i = i->next)
		;
	i->next = n;
	return p;
}

Node *lookup(Node *p, Node *n, int (*cmp)(void*, void*))
{
	for (; p != NULL; p = p->next) {
		if ((*cmp)(p->data, n->data) == 0)
			return p;
	}
	return NULL;
}

Node *delnode(Node *p, Node *n, int (*cmp)(void*, void*), void (*freedata)(void*))
{
	Node* i; Node* prev = NULL;
	for (i = p; i; i = i->next) {
		if ((*cmp)(i->data, n->data) == 0) {
			if (prev == NULL)
				p = i->next;
			else
				prev->next = i->next;
			freenode(i, freedata);
			return p;
		}
		prev = i;
	}
	return NULL;
}

void apply(Node *p, void (*fn)(void*, void*), void *arg)
{
	if (fn == NULL)
		return;
	for (; p != NULL; p = p->next)
		(*fn)(arg, p->data);
}

void inccounter(void *arg, void *data)
{
	int *c;
	c = (int *)arg;
	(*c)++;
}
