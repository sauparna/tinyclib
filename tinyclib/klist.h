#pragma once
typedef struct Node Node;

struct Node {
	void* data;
	Node* next;
};

Node* newnode(void*);
void freenode(Node*, void (*freedata)(void*));
Node* addfront(Node*, Node*);
Node* addend(Node*, Node*);
Node* lookup(Node*, Node*, int (*cmp)(void*, void*));
void apply(Node*, void (*fn)(void*, void*), void*);
Node *delnode(Node*, Node*, int (*cmp)(void*, void*), void (*freedata)(void*));
void freelist(Node*, void (*freedata)(void*));
void inccounter(void*, void*);
