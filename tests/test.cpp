#include "pch.h"
#include "../tinyclib/klist.h"
#include "../tinyclib/kpost.h"
#include "../tinyclib/ktree.h"
#include "../tinyclib/khash.h"

// ============================================= LIST TESTS ======================================================//

#define NUM 10
#define MAXLEN 10
char s[NUM][MAXLEN] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };

TEST(KListTest, addfront)
{
	Node* list = NULL;

	for (int i = 0; i < 3; i++)
		list = addfront(list, newnode((void*)newkpost(s[i], i)));
	char expected[NUM][MAXLEN] = { "c", "b", "a" };
	int i = 0;
	for (Node* p = list; p; p = p->next, ++i)
		EXPECT_STREQ((KPOST(p->data))->s, expected[i]);
}

TEST(KListTest, addend)
{
	Node* list = NULL;

	for (int i = 0; i < 3; i++)
		list = addend(list, newnode((void*)newkpost(s[i], i)));
	char expected[NUM][MAXLEN] = { "a", "b", "c" };
	int i = 0;
	for (Node* p = list; p; p = p->next, ++i)
		EXPECT_STREQ((KPOST(p->data))->s, expected[i]);
}

TEST(KListTest, lookup)
{
	int n = 3;
	Node* list = NULL;
	for (int i = 0; i < n; i++)
		list = addfront(list, newnode((void*)newkpost(s[i], i)));
	Node* a = (Node*)newnode((void*)newkpost(s[0], 0));
	Node* a_ = lookup(list, a, cmpkpost);
	ASSERT_STREQ((KPOST(a_->data))->s, (KPOST(a->data))->s);
	Node* d = (Node*)newnode((void*)newkpost(s[n], 0));
	Node* d_ = lookup(list, d, cmpkpost);
	ASSERT_EQ(d_, nullptr);
}

TEST(KListTest, delnode)
{
	int n = 3;
	Node* list = NULL;
	for (int i = 0; i < n; i++)
		list = addfront(list, newnode((void*)newkpost(s[i], i)));
	Node* a = (Node*)newnode((void*)newkpost(s[0], 0));
	Node* b = (Node*)newnode((void*)newkpost(s[1], 1));
	Node* c = (Node*)newnode((void*)newkpost(s[2], 2));

	int i;
	list = delnode(list, b, cmpkpost, freekpost);
	char expected1[NUM][MAXLEN] = { "c", "a" };
	i = 0;
	for (Node* p = list; p; p = p->next, ++i)
		EXPECT_STREQ((KPOST(p->data))->s, expected1[i]);

	list = delnode(list, c, cmpkpost, freekpost);
	char expected2[NUM][MAXLEN] = { "a" };
	i = 0;
	for (Node* p = list; p; p = p->next, ++i)
		EXPECT_STREQ((KPOST(p->data))->s, expected2[i]);

	list = delnode(list, a, cmpkpost, freekpost);
	ASSERT_EQ(list, nullptr);
}

TEST(KListTest, apply)
{
	int n = 3;
	Node* list = NULL;
	for (int i = 0; i < n; i++)
		list = addfront(list, newnode((void*)newkpost(s[i], i)));
	int c = 0;
	apply(list, inccounter, &c);
	ASSERT_EQ(c, n);
}

// ============================================= TREE TESTS ======================================================//
#define NUMTREENODES 10
enum TreeNodeIndex { A, B, C, D, E, F, G, H, I, J };
char tree_node_name[NUMTREENODES][MAXLEN] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };
TNode* tree[NUMTREENODES];

void init_tree()
{
	// This tree is from TAOCP 2.3.1 319

	for (int i = 0; i < NUMTREENODES; ++i)
		tree[i] = newtnode(&tree_node_name[i]);

	tree[A]->llink = tree[B];
	tree[A]->rlink = tree[C];

	tree[B]->llink = tree[D];
	tree[B]->rlink = nullptr;

	tree[C]->llink = tree[E];
	tree[C]->rlink = tree[F];

	tree[D]->llink = nullptr;
	tree[D]->rlink = nullptr;

	tree[E]->llink = nullptr;
	tree[E]->rlink = tree[G];

	tree[F]->llink = tree[H];
	tree[F]->rlink = tree[J];

	tree[G]->llink = nullptr;
	tree[G]->rlink = nullptr;

	tree[H]->llink = nullptr;
	tree[H]->rlink = nullptr;

	tree[J]->llink = nullptr;
	tree[J]->rlink = nullptr;
}

void build_path_string(void* arg, void* data)
{
	std::string* s = (std::string*)arg;
	s->append((char*)data);
}

int cmp_tree_node(void* data, void* data1)
{
	return strcmp((char*)data, (char*)data1);
}

TEST(KTreeTest, init)
{
	init_tree();
	ASSERT_TRUE(true);
}

TEST(KTreeTest, preorder)
{
	// A B D C E G F H J
	std::string s{};
	applypreorder(tree[A], build_path_string, &s);
	std::string expected{ "ABDCEGFHJ" };
	EXPECT_EQ(s, expected);
}

TEST(KTreeTest, inorder)
{
	// D B A E G C H F J
	std::string s{};
	applyinorder(tree[A], build_path_string, &s);
	std::string expected{ "DBAEGCHFJ" };
	EXPECT_EQ(s, expected);
}

TEST(KTreeTest, postorder)
{
	// D B G E H J F C A
	std::string s{};
	applypostorder(tree[A], build_path_string, &s);
	std::string expected{ "DBGEHJFCA" };
	EXPECT_EQ(s, expected);
}

TEST(KTreeTest, tree_lookup)
{
	TNode* found_node = NULL;
	found_node = tlookup(tree[A], tree[I], cmp_tree_node);
	ASSERT_EQ(found_node, nullptr);
	found_node = tlookup(tree[A], tree[J], cmp_tree_node);
	ASSERT_EQ(found_node, tree[J]);
}

TEST(KTreeTest, non_recursive_tree_lookup)
{
	TNode* found_node = NULL;
	found_node = tlookup(tree[A], tree[I], cmp_tree_node);
	ASSERT_EQ(found_node, nullptr);
	found_node = tlookup(tree[A], tree[J], cmp_tree_node);
	ASSERT_EQ(found_node, tree[J]);
}

TEST(KTreeTest, insert)
{
	insert(tree[A], tree[I], cmp_tree_node, NULL);
	ASSERT_EQ(tree[J]->llink, tree[I]);
	tree[J]->llink = nullptr;
}

TEST(KTreeTest, non_recursive_insert)
{
	nrinsert(tree[A], tree[I], cmp_tree_node, NULL);
	ASSERT_EQ(tree[J]->llink, tree[I]);
	tree[J]->llink = nullptr;
}

// ============================================= HASH TABLE TESTS ======================================================//

#define TABSIZE 3
#define NUMELEMENTS 6
enum HastTableNodeIndex {CAT, MAT, BAT, HAT, RAT, SAT};
char tab_strings[NUMELEMENTS][MAXLEN] = { "cat", "mat", "bat", "hat", "rat", "sat" };
Node* ntab[NUMELEMENTS];
KHash* htab;

void init_hash()
{
	for (int i = 0; i < NUMELEMENTS; ++i)
		ntab[i] = newnode(&tab_strings[i]);
	htab = newhash(TABSIZE, _strcmp, _strhash);
	for (int i = 0; i < NUMELEMENTS - 1; ++i)
		hlookup(htab, ntab[i], 1);
}

TEST(KHashTest, init)
{
	init_hash();
	ASSERT_TRUE(true);
}

TEST(KHashTest, hlookup)
{
	Node* found_node = nullptr;
	found_node = hlookup(htab, ntab[CAT], 0);
	ASSERT_EQ(found_node, ntab[CAT]);
	found_node = hlookup(htab, ntab[SAT], 0);
	ASSERT_EQ(found_node, nullptr);
}

// ========================================================================================================================//