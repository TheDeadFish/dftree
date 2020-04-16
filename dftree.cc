#include <stdio.h>
#include <assert.h>
#include "dftree.h"

// stdshit bits
#define IFRET(...) ({ if(auto x = __VA_ARGS__) return x; })
#define VARFIX(x) asm("" : "+r"(x))

/* Internal utility macros. */
#define rbtn_first(a_rbt, a_root, r_node) do {		\
    (r_node) = (a_root);						\
    if ((r_node) != NULL) {						\
	for (;								\
	  (r_node)->left != NULL;		\
	  (r_node) = (r_node)->left) {	\
	}								\
    }									\
} while (0)

#define rbtn_last(a_rbt, a_root, r_node) do {		\
    (r_node) = (a_root);						\
    if ((r_node) != NULL) {						\
	for (; (r_node)->right != NULL;	\
	  (r_node) = (r_node)->right) {	\
	}								\
    }									\
} while (0)

#define rbtn_rotate_left(a_node, r_node) do {		\
    (r_node) = (a_node)->right;		\
    (a_node)->right = (r_node)->left;			\
    (r_node)->left = (a_node);			\
} while (0)

#define rbtn_rotate_right(a_node, r_node) do {		\
    (r_node) = (a_node)->left;		\
    (a_node)->left = (r_node)->right; \
		(r_node)->right = (a_node);		\
} while (0)



dfnode* dftree_path::init(dfnode* node, void* key, compar_t key_cmp)
{
	list_t* pathp = path;
	for(; pathp->node = node; pathp++) 
	{
		int cmp = pathp->cmp = key_cmp(key, pathp->node);
		if(cmp == 0) break;
		if (cmp < 0) { node = node->left;
		} else { node = node->right; }
	}
	
	this->pathp = pathp;
	return node;
}

void dftree_path::insert(dftree* tree, dfnode* inode)
{
	list_t* pathp = this->pathp;
	inode->init();
	
	while(chk(--pathp))
	{
		dfnode *cnode = pathp->node;
		if (pathp->cmp < 0) {
			dfnode *left = inode;
			cnode->left = left;
			if (left->color()) {
				dfnode *leftleft = left->left;
				if (leftleft != NULL && leftleft->color()) {

					dfnode *tnode;
					leftleft->black_set();
					rbtn_rotate_right(cnode, tnode);
					cnode = tnode;
				}
			} else {
				return;
			}
		} else {
			dfnode *right = inode;
			cnode->right = right;
			if (right->color()) {
				dfnode *left = cnode->left;
				if (left != NULL && left->color()) {

					left->black_set();
					right->black_set();
					cnode->red_set();
				} else {

					dfnode *tnode;
					bool tred = cnode->color();
					rbtn_rotate_left(cnode, tnode);
					tnode->color_set(tred);
					cnode->red_set();
					cnode = tnode;
				}
			} else {
				return;
			}
		}
		inode = cnode;
	}

	inode->black_set();
	tree->root = inode;
}

dftree_insret_t dftree_insert(dftree *rbtree, 
	void *knode, void* ctx, compar_t key_cmp,
	dfnode* (__fastcall *node_create)(void* ctx, void *key))
{
	// find the node
	dftree_path path;
	dfnode* node = path.init(rbtree->root, knode, key_cmp);
	if(node) return {node, false};
	
	// create the node
	dfnode* inode = (dfnode*)knode;
	if(node_create) inode = node_create(ctx, knode);
	path.insert(rbtree, inode);
	return {inode, true};
};

void dftree_insert(dftree *rbtree, dfnode *inode, compar_t key_cmp)
{
	dftree_path path;
	dfnode* fnode = path.init(rbtree->root, inode, key_cmp);
	assert(fnode == NULL);
	path.insert(rbtree, inode);
}

typedef void* (__fastcall *dftree_iter_t)(void*, dfnode*);

void* dftree_iter_recurse_(
	dfnode* node, void* ctx, dftree_iter_t cb)
{
	if(!node) return node;
	IFRET(dftree_iter_recurse_(node->left, ctx, cb));
	IFRET(cb(ctx, node));
	return dftree_iter_recurse_(node->right, ctx, cb);
}

void* dftree_iter_rrecurse_(
	dfnode* node, void* ctx, dftree_iter_t cb)
{
	if(!node) return node;
	IFRET(dftree_iter_recurse_(node->right, ctx, cb));
	IFRET(cb(ctx, node));
	return dftree_iter_recurse_(node->left, ctx, cb);
}


	
dfnode* dftree_search(dfnode* node,
	const void* key, compar_t key_cmp)
{
	while (node != NULL) {
		int cmp = key_cmp(key, node);
		if(cmp > 0) node = node->right;
		else { if(!cmp) break;
			node = node->left; }
	}
	return node;
}

dfnode* dftree_nsearch(dfnode* node,
	const void* key, compar_t key_cmp)
{
	dfnode* ret = NULL;
	while (node != NULL) {
		int cmp = key_cmp(key, node);
		if(cmp > 0) node = node->right;
		else { ret = node; if(!cmp) break;
			node = node->left; }
	}
	return ret;
}

dfnode* dftree_psearch(dfnode* node,
	const void* key, compar_t key_cmp)
{
	dfnode* ret = NULL;
	while (node != NULL) {
		int cmp = key_cmp(key, node);
		if(cmp < 0) node = node->left;
		else { ret = node; if(!cmp) break;
			node = node->right; }
	}
	return ret;
}
