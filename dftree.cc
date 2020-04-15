#include <stdio.h>
#include <assert.h>
#include "dftree.h"

/* Internal utility macros. */
#define rbtn_first(a_rbt, a_root, r_node) do {		\
    (r_node) = (a_root);						\
    if ((r_node) != NULL) {						\
	for (;								\
	  (r_node)->left() != NULL;		\
	  (r_node) = (r_node)->left()) {	\
	}								\
    }									\
} while (0)

#define rbtn_last(a_rbt, a_root, r_node) do {		\
    (r_node) = (a_root);						\
    if ((r_node) != NULL) {						\
	for (; (r_node)->right() != NULL;	\
	  (r_node) = (r_node)->right()) {	\
	}								\
    }									\
} while (0)

#define rbtn_rotate_left(a_node, r_node) do {		\
    (r_node) = (a_node)->right();		\
    (a_node)->right_set((r_node)->left());			\
    (r_node)->left_set((a_node));			\
} while (0)

#define rbtn_rotate_right(a_node, r_node) do {		\
    (r_node) = (a_node)->left();		\
    (a_node)->left_set((r_node)->right()); \
		(r_node)->right_set((a_node));		\
} while (0)


dftree_insret_t dftree_insert(dftree *rbtree, 
	void *knode, void* ctx, compar_t key_cmp,
	dfnode* (__fastcall *node_create)(void* ctx, void *key))
{

	struct {
		dfnode *node;
		int cmp;
	} path[sizeof(void *) << 4], *pathp;
	
	// locate insertion point
	path->node = rbtree->root;
	for (pathp = path; pathp->node != NULL; pathp++) {
		int cmp = pathp->cmp = key_cmp(knode, pathp->node);
		if(cmp == 0) return {pathp->node, false};
		if (cmp < 0) {
			pathp[1].node = pathp->node->left();
		} else {
			pathp[1].node = pathp->node->right();
		}
	}
	
	// create the node
	dfnode* inode = (dfnode*)knode;
	if(node_create) inode = node_create(ctx, knode);
	pathp->node = inode; inode->init();

	for (pathp--; (uintptr_t)pathp >= (uintptr_t)path; pathp--) {
		dfnode *cnode = pathp->node;
		if (pathp->cmp < 0) {
			dfnode *left = pathp[1].node;
			cnode->left_set(left);
			if (left->color()) {
				dfnode *leftleft = left->left();
				if (leftleft != NULL && leftleft->color()) {

					dfnode *tnode;
					leftleft->black_set();
					rbtn_rotate_right(cnode, tnode);
					cnode = tnode;
				}
			} else {
				return {inode, true};
			}
		} else {
			dfnode *right = pathp[1].node;
			cnode->right_set(right);
			if (right->color()) {
				dfnode *left = cnode->left();
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
				return {inode, true};
			}
		}
		pathp->node = cnode;
	}

	rbtree->root = path->node;
	rbtree->root->black_set();
	return {inode, true};
};




typedef void* (__fastcall *dftree_iter_t)(void*, dfnode*);

__fastcall void* dftree_iter_recurse_(
	dfnode* node, void* ctx, dftree_iter_t cb)
{
	if(!node) return node;
	IFRET(dftree_iter_recurse_(node->left(), ctx, cb));
	IFRET(cb(ctx, node));
	return dftree_iter_recurse_(node->right(), ctx, cb);
}

__fastcall void* dftree_iter_rrecurse_(
	dfnode* node, void* ctx, dftree_iter_t cb)
{
	if(!node) return node;
	IFRET(dftree_iter_recurse_(node->right(), ctx, cb));
	IFRET(cb(ctx, node));
	return dftree_iter_recurse_(node->left(), ctx, cb);
}


	
dfnode* dftree_search(dfnode* node,
	const void* key, compar_t key_cmp)
{
	while (node != NULL) {
		int cmp = key_cmp(key, node);
		if(cmp > 0) node = node->right();
		else { if(!cmp) break;
			node = node->left(); }
	}
	return node;
}

dfnode* dftree_nsearch(dfnode* node,
	const void* key, compar_t key_cmp)
{
	dfnode* ret = NULL;
	while (node != NULL) {
		int cmp = key_cmp(key, node);
		if(cmp > 0) node = node->right();
		else { ret = node; if(!cmp) break;
			node = node->left(); }
	}
	return ret;
}

dfnode* dftree_psearch(dfnode* node,
	const void* key, compar_t key_cmp)
{
	dfnode* ret = NULL;
	while (node != NULL) {
		int cmp = key_cmp(key, node);
		if(cmp < 0) node = node->left();
		else { ret = node; if(!cmp) break;
			node = node->right(); }
	}
	return ret;
}
