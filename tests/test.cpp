#include "pch.h"
#include "../tinyclib/klist.h"
#include "../tinyclib/ktree.h"
#include "../tinyclib/khash.h"

// ============================================= LIST TESTS ======================================================//

#define NUM 10
#define MAXLEN 10
#define LISTSIZE 4
char list_data[NUM][MAXLEN] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };

Node* list = NULL;

TEST(KListTest, addfront)
{
	for (int i = 0; i < LISTSIZE - 1; i++)
		list = addfront(list, newnode(&list_data[i]));
	char expected[LISTSIZE - 1][MAXLEN] = { "c", "b", "a" };
	int i = 0;
	for (Node* p = list; p; p = p->next, ++i)
		EXPECT_STREQ((char*)(p->data), expected[i]);
}

TEST(KListTest, addend)
{
	list = addend(list, newnode(&list_data[LISTSIZE - 1]));
	char expected[LISTSIZE][MAXLEN] = { "c", "b", "a", "d" };
	int i = 0;
	for (Node* p = list; p; p = p->next, ++i)
		EXPECT_STREQ((char*)(p->data), expected[i]);
}

TEST(KListTest, lookup)
{
	Node* p = nullptr;
	Node* q = nullptr;
	p = (Node*)newnode(&list_data[0]);
	q = lookup(list, p, _strcmp);
	ASSERT_STREQ((char*)(p->data), (char*)(q->data));
	p = (Node*)newnode(&list_data[LISTSIZE + 1]);
	q = lookup(list, p, _strcmp);
	ASSERT_EQ(q, nullptr);
}

TEST(KListTest, apply)
{
	int n = 4;
	int c = 0;
	apply(list, inccounter, &c);
	ASSERT_EQ(c, n);
}

TEST(KListTest, delnode)
{
	Node* a = (Node*)newnode(&list_data[0]);
	Node* b = (Node*)newnode(&list_data[1]);
	Node* c = (Node*)newnode(&list_data[2]);
	Node* d = (Node*)newnode(&list_data[3]);

	int i;
	list = delnode(list, b, _strcmp, NULL);
	char expected1[LISTSIZE - 1][MAXLEN] = { "c", "a", "d" };
	i = 0;
	for (Node* p = list; p; p = p->next, ++i)
		EXPECT_STREQ((char*)(p->data), expected1[i]);

	list = delnode(list, c, _strcmp, NULL);
	char expected2[LISTSIZE - 2][MAXLEN] = { "a", "d" };
	i = 0;
	for (Node* p = list; p; p = p->next, ++i)
		EXPECT_STREQ((char*)(p->data), expected2[i]);

	list = delnode(list, a, _strcmp, NULL);
	list = delnode(list, d, _strcmp, NULL);
	ASSERT_EQ(list, nullptr);
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
	found_node = tlookup(tree[A], tree[I], _strcmp);
	ASSERT_EQ(found_node, nullptr);
	found_node = tlookup(tree[A], tree[J], _strcmp);
	ASSERT_EQ(found_node, tree[J]);
}

TEST(KTreeTest, non_recursive_tree_lookup)
{
	TNode* found_node = NULL;
	found_node = tlookup(tree[A], tree[I], _strcmp);
	ASSERT_EQ(found_node, nullptr);
	found_node = tlookup(tree[A], tree[J], _strcmp);
	ASSERT_EQ(found_node, tree[J]);
}

TEST(KTreeTest, insert)
{
	insert(tree[A], tree[I], _strcmp, NULL);
	ASSERT_EQ(tree[J]->llink, tree[I]);
	tree[J]->llink = nullptr;
}

TEST(KTreeTest, non_recursive_insert)
{
	nrinsert(tree[A], tree[I], _strcmp, NULL);
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