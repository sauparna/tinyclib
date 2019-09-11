#pragma once
typedef struct TNode TNode;

struct TNode {
	void *data;
	TNode *llink;
	TNode *rlink;
};

TNode *newtnode(void *);
void  freetnode(TNode *, void (*freedata)(void *));
TNode *insert(TNode *, TNode *, int (*cmp)(void *, void *), int (*match_handler)(void *, void *));
TNode *nrinsert(TNode *, TNode *, int (*cmp)(void *, void *), int (*match_handler)(void *, void *));
TNode *tlookup(TNode *, TNode *, int (*cmp)(void *, void *));
TNode *nrtlookup(TNode *, TNode *, int (*cmp)(void *, void *));
void applypreorder(TNode* p, void (*fn)(void*, void*), void* arg);
void  applyinorder(TNode *, void (*fn)(void *, void *), void *);
void  applypostorder(TNode *, void (*fn)(void *, void *), void *);
