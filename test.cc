#include <stdio.h>
#include <assert.h>

#define RB_COMPACT
#include "rb.h"

typedef struct node_s node_t;

struct node_s {
	rb_node(node_t) link;
	
	node_s* left() { return link.rbn_left; }
	node_s* right() { return (node_s*)(intptr_t(
		link.rbn_right_red) & ((ssize_t)-2)); }
	
	int key;
};


static int
node_cmp(const node_t *a, const node_t *b) {
	return a->key - b->key;
}

typedef rb_tree(node_t) tree_t;


#include "rb-x-x.cc"


node_s nodes[10000];

void traversi(node_s* node, int depth)
{
	if(node == NULL) return;
	printf("%d : %d\n", depth, node->key);
	
	traversi(node->left(), depth+1);
	traversi(node->right(), depth+1);
	
	
	




}


int main()
{
	



	tree_t tree;
	tree_new(&tree);
	for(int i = 0; i < 20; i++) {
		nodes[i].key = i*10;
		tree_insert(&tree, &nodes[i]);
	}
	
	traversi(tree.rbt_root, 0);
	
	
	
	
}

