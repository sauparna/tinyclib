#include <stdio.h>
#include <stdlib.h>
#include "kcommon.h"
#include "ktree.h"

TNode *newtnode(void *data)
{
	TNode *n;
	n = (TNode *)kmalloc(sizeof(TNode));
	n->data = data;
	n->llink = NULL;
	n->rlink = NULL;
	return n;
}

void freetnode(TNode *n, void (*freedata)(void*))
{
	if (freedata != NULL)
		(*freedata)(n->data);
	free(n);
}

TNode *insert(TNode *treep, TNode *newp, int (*cmp)(void*, void*), int (*match_handler)(void*, void*))
{
	int ret;
	if (treep == NULL)
		return newp;
	ret = (*cmp)(newp->data, treep->data);
	if (ret == 0) {
		if (match_handler != NULL)
			(*match_handler)(newp->data, treep->data);
	}
	else if (ret < 0)
		treep->llink = insert(treep->llink, newp, cmp, match_handler);
	else
		treep->rlink = insert(treep->rlink, newp, cmp, match_handler);
	return treep;
}

TNode *nrinsert(TNode *treep, TNode *newp, int (*cmp)(void*, void*), int (*match_handler)(void*, void*))		
{
	TNode **p;
	int ret;
	if (treep == NULL)
		return newp;
	p = &treep;
	while (*p != NULL) {
		ret = (*cmp)(newp->data, (*p)->data);
		if (ret == 0) {
			if (match_handler != NULL)
				(*match_handler)(newp->data, (*p)->data);
			return treep;
		}
		else if (ret < 0)
			p = &((*p)->llink);
		else
			p = &((*p)->rlink);
	}
	*p = newp;
	return treep;
}

TNode *tlookup(TNode *p, TNode *n, int (*cmp)(void*, void*))
{
	int ret;
	if (p == NULL)
		return NULL;
	ret = (*cmp)(n->data, p->data);
	if (ret == 0)
		return p;
	else if (ret < 0)
		return tlookup(p->llink, n, cmp);
	else
		return tlookup(p->rlink, n, cmp);
}

TNode *nrtlookup(TNode *treep, TNode *newp, int (*cmp)(void*, void*))
{
	int ret;
	TNode **p;
	p = &treep;
	while (*p != NULL) {
		ret = (*cmp)(newp->data, (*p)->data);
		if (ret == 0)
			return *p;
		else if (ret < 0)
			p = &((*p)->llink);
		else
			p = &((*p)->rlink);
	}
	return NULL;
}

void applypreorder(TNode* p, void (*fn)(void*, void*), void* arg)
{
	if (p == NULL)
		return;
	(*fn)(arg, p->data);
	applypreorder(p->llink, fn, arg);
	applypreorder(p->rlink, fn, arg);
}

void applyinorder(TNode* p, void (*fn)(void*, void*), void *arg)
{
	if (p == NULL)
		return;
	applyinorder(p->llink, fn, arg);
	(*fn)(arg, p->data);
	applyinorder(p->rlink, fn, arg);	
}

void applypostorder(TNode* p, void (*fn)(void*, void*), void *arg)
{
	if (p == NULL)
		return;
	applypostorder(p->llink, fn, arg);
	applypostorder(p->rlink, fn, arg);
	(*fn)(arg, p->data);
}
