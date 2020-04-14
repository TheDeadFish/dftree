#include <stdio.h>
#include <assert.h>

#define RB_COMPACT
#include "rb.h"
#include "stuff.h"
#include "dftree.h"

struct node_s {
	dfnode link;
	
	static node_s* get(const dfnode* x) { return 
		container_of((dfnode*)x, &node_s::link); }
	int key;
};


static int
node_cmp_(const node_s *a, const node_s *b) {
	return a->key - b->key;
}

static int
node_cmp(const dfnode *a, const dfnode *b) {
	return node_cmp_(node_s::get(a), node_s::get(b));
}


#include "rb-x-x.cc"


node_s nodes[10000];

void traversi(dfnode* node, int depth)
{
	if(node == NULL) return;
	printf("%d : %d\n", depth, node_s::get(node)->key);
	
		
	
	traversi(node->rbn_left, depth+1);
	traversi(node->right_get(), depth+1);
	
	
	




}


int main()
{
	



	dftree tree;
	tree_new(&tree);
	for(int i = 0; i < 20; i++) {
		nodes[i].key = i*10;
		
		dftree_insert(&tree, &nodes[i].link, COMPAR(node_cmp));
	}
	
	traversi(tree.rbt_root, 0);
	
	
	
	
}

