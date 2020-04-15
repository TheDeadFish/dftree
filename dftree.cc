#include <stdio.h>
#include <assert.h>


#include "rb.h"
#include "dftree.h"


/* Internal utility macros. */
#define rbtn_first(dfnode, a_field, a_rbt, a_root, r_node) do {		\
    (r_node) = (a_root);						\
    if ((r_node) != NULL) {						\
	for (;								\
	  rbtn_left_get(dfnode, a_field, (r_node)) != NULL;		\
	  (r_node) = rbtn_left_get(dfnode, a_field, (r_node))) {	\
	}								\
    }									\
} while (0)

#define rbtn_last(dfnode, a_field, a_rbt, a_root, r_node) do {		\
    (r_node) = (a_root);						\
    if ((r_node) != NULL) {						\
	for (; rbtn_right_get(dfnode, a_field, (r_node)) != NULL;	\
	  (r_node) = rbtn_right_get(dfnode, a_field, (r_node))) {	\
	}								\
    }									\
} while (0)

#define rbtn_rotate_left(dfnode, a_field, a_node, r_node) do {		\
    (r_node) = rbtn_right_get(dfnode, a_field, (a_node));		\
    rbtn_right_set(dfnode, a_field, (a_node),				\
      rbtn_left_get(dfnode, a_field, (r_node)));			\
    rbtn_left_set(dfnode, a_field, (r_node), (a_node));			\
} while (0)

#define rbtn_rotate_right(dfnode, a_field, a_node, r_node) do {		\
    (r_node) = rbtn_left_get(dfnode, a_field, (a_node));		\
    rbtn_left_set(dfnode, a_field, (a_node),				\
      rbtn_right_get(dfnode, a_field, (r_node)));			\
    rbtn_right_set(dfnode, a_field, (r_node), (a_node));		\
} while (0)


bool dftree_insert(dftree *rbtree, 
	void *knode, void* ctx, compar_t key_cmp,
	dfnode* (__fastcall *node_create)(void* ctx, void *key))
{

	struct {
		dfnode *node;
		int cmp;
	} path[sizeof(void *) << 4], *pathp;
	
	// locate insertion point
	path->node = rbtree->rbt_root;
	for (pathp = path; pathp->node != NULL; pathp++) {
		int cmp = pathp->cmp = key_cmp(knode, pathp->node);
		if(cmp == 0) return false;
		if (cmp < 0) {
			pathp[1].node = rbtn_left_get(dfnode, link, pathp->node);
		} else {
			pathp[1].node = rbtn_right_get(dfnode, link, pathp->node);
		}
	}
	
	// create the node
	if(node_create) {
		pathp->node = node_create(ctx, knode);
	} else { pathp->node = (dfnode*)knode; }
	pathp->node->init();

	for (pathp--; (uintptr_t)pathp >= (uintptr_t)path; pathp--) {
		dfnode *cnode = pathp->node;
		if (pathp->cmp < 0) {
			dfnode *left = pathp[1].node;
			rbtn_left_set(dfnode, link, cnode, left);
			if (rbtn_red_get(dfnode, link, left)) {
				dfnode *leftleft = rbtn_left_get(dfnode, link, left);
				if (leftleft != NULL && rbtn_red_get(dfnode, link, leftleft)) {

					dfnode *tnode;
					rbtn_black_set(dfnode, link, leftleft);
					rbtn_rotate_right(dfnode, link, cnode, tnode);
					cnode = tnode;
				}
			} else {
				return true;
			}
		} else {
			dfnode *right = pathp[1].node;
			rbtn_right_set(dfnode, link, cnode, right);
			if (rbtn_red_get(dfnode, link, right)) {
				dfnode *left = rbtn_left_get(dfnode, link, cnode);
				if (left != NULL && rbtn_red_get(dfnode, link, left)) {

					rbtn_black_set(dfnode, link, left);
					rbtn_black_set(dfnode, link, right);
					rbtn_red_set(dfnode, link, cnode);
				} else {

					dfnode *tnode;
					bool tred = rbtn_red_get(dfnode, link, cnode);
					rbtn_rotate_left(dfnode, link, cnode, tnode);
					rbtn_color_set(dfnode, link, tnode, tred);
					rbtn_red_set(dfnode, link, cnode);
					cnode = tnode;
				}
			} else {
				return true;
			}
		}
		pathp->node = cnode;
	}

	rbtree->rbt_root = path->node;
	rbtn_black_set(dfnode, link, rbtree->rbt_root);
	return true;
};
