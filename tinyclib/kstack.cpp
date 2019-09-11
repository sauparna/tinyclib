#include <stdlib.h>
#include "klist.h"
#include "kstack.h"

void initstack(Stack *stk)
{
	stk->top = NULL;
	stk->n = 0;
}

void push(Stack *stk, void *data)
{
	Node *n;
	n = newnode(data);
	stk->top = addfront(stk->top, n);
	stk->n++;
}

void *pop(Stack *stk)
{
	Node *top;
	void *data;
	if (stk->top != NULL) {
		top = stk->top;
		data = stk->top->data;
		stk->top = stk->top->next;
		free(top);
		stk->n--;
		return data;
	}
	return NULL;
}

void purge(Stack *stk)
{
	Node *next = NULL;
	for (; stk->top != NULL; stk->top = next) {
		next = stk->top->next;
		free(stk->top);
	}
	stk->top = NULL;
	stk->n = 0;
}
