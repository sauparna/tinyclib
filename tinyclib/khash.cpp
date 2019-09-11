#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kcommon.h"
#include "klist.h"
#include "khash.h"

KHash *newhash(unsigned n, fn_cmp cmp, fn_hash hash) {
	KHash *h;
	h       = (KHash *)kmalloc(sizeof(KHash));
	h->tab  = (Node **)kmalloc(sizeof(Node *) * n);
	for (int i = 0; i < n; i++)
		h->tab[i] = NULL;
	h->nel  = 0;
	h->n    = n;
	h->cmp  = cmp;
	h->hash = hash;
	return h;
}

void freehash(KHash *h, void (*freedata)(void*))
{
	if (h == NULL)
		return;
	for (int i = 0; i < h->n; i++)
		freelist(h->tab[i], freedata);
	free(h);
}

int _strcmp(void *d1, void *d2)
{
	return strcmp((char *)d1, (char *)d2);
}

unsigned _strhash(void *data, unsigned size)
{
	static unsigned MULTIPLIER = 31;
	unsigned h;
	h = 0;
	for (unsigned char *p = (unsigned char*)data; *p != '\0'; p++)
		h = MULTIPLIER * h + *p;
	return h % size;
}

Node *hlookup(KHash *h, Node *n, int create)
{
	Node *np;
	unsigned m;
	m = (*h->hash)(n->data, h->n);
	for (np = h->tab[m]; np != NULL; np = np->next)
		if ((*h->cmp)(n->data, np->data) == 0)
			return np;
	if (create == 1) {
		h->tab[m] = addfront(h->tab[m], n);
		h->nel++;
		return h->tab[m];
	}
	return NULL;
}

void fprinthashstats(FILE *stream, KHash *h)
{
	float freebuckets, avgchainlen;
	int chains, min, max, chainlen, buckets, *bucket;
	chains = min = max = 0;
	buckets = 30;
	bucket = (int *)malloc(sizeof(int) * (buckets + 1));
	memset(bucket, 0, sizeof(int) * (buckets + 1));

	for (int i = 0; i < h->n; i++) {
		chainlen = 0;
		for (Node *np = h->tab[i]; np != NULL; np = np->next)
			chainlen++;
		if (chainlen > 0) chains++;
		if (chainlen > max) max = chainlen;
		if (chainlen < min) min = chainlen;
		if (chainlen >= buckets)
			bucket[buckets]++;
		else
			bucket[chainlen]++;
	}

	freebuckets = (float)(h->n - chains) / (float)h->n * 100.0f;
	avgchainlen  = (float)h->nel / (float)chains;
	fprintf(stream, "unused   : %-4.2f%% (%d/%d)\n",freebuckets, h->n - chains, h->n);
	fprintf(stream, "elements : %-d\n", h->nel);
	fprintf(stream, "chain avg: %-5.1f\n", avgchainlen);
	fprintf(stream, "chain max: %-d\n", max);
	fprintf(stream, "chain min: %-d\n", min);

	/* plot bucket */
	for (int i = 0; i < buckets; i++) {
		if (i%10 == 0)
			printf("%-3d - ", i);
		else
			printf("%-3s | ", " ");
		float y = (float)bucket[i] / (float)buckets * 40.0f;
		printf("%d", (int)y);
/*		for (int j = 0; j < (int)y; j++)
			printf("*");
*/		printf("\n");
	}

	free(bucket);
}

void fprinthash(FILE *stream, KHash *h, void (*printdata)(void*, void*))
{
	int flag;
	for (int i = 0; i < h->n; i++) {
		flag = 0;
		for (Node *np = h->tab[i]; np != NULL; np = np->next) {
			if (flag == 0) {
				fprintf(stream, "%10d ", i);
				flag = 1;
			}
			else
				fprintf(stream, "\n%-10s ", " ");
			(*printdata)(stream, np->data);
		}
		fprintf(stream, "\n");
	}
}
