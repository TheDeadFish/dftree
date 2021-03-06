#include <stdio.h>
#include <assert.h>

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


//#include "rb-x-x.cc"


node_s nodes[10000];

void traversi(dfnode* node, int depth)
{
	if(node == NULL) return;
	printf("%d : %d\n", depth, node_s::get(node)->key);
	
		
	
	traversi(node->left, depth+1);
	traversi(node->right, depth+1);
	
	
	




}


void* __fastcall trav_cb(void* ctx, dfnode* node)
{
	auto* n = node->cont(&node_s::link);
	


	printf("%d\n", n->key);
	
	return NULL;
}

template <class T>
void* dftree_iter(dfnode* node, void* ctx, T cb) {
	return dftree_iter_recurse_(node, ctx, (dftree_iter_t)cb); }



static 
int compar(int key, node_s* node) { return key - node->key; }




int main()
{
	



	dftree tree = {};

	for(int i = 0; i < 20; i++) {
		nodes[i*2+0].key = i*10;
		
		dftree_insert(&tree, &nodes[i].link, 0, COMPAR(node_cmp), 0);
		nodes[i*2+1].key = -i*10;
		dftree_insert(&tree, &nodes[i].link, 0, COMPAR(node_cmp), 0);
		
	}
	
	traversi(tree.root, 0);
	
	dftree_iter(tree.root, 0, trav_cb);
	
	printf("hello\n");
	
	
	dftree_path path;
	
	for(int i = 0; i < 50; i += 5) {
		auto x = path.ninit(tree, (void*)i, COMPAR(compar));

		printf("%d: %d, %d\n", i, 
			(x.node ? (((node_s*)(x.node))->key) : -1), x.found);

	}
	
	
	
	
}

