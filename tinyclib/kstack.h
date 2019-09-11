#pragma once
typedef struct Stack Stack;

struct Stack {
	Node *top;
	int n;
};

void initstack(Stack *);
void push(Stack *, void *);
void *pop(Stack *);
void purge(Stack *);
