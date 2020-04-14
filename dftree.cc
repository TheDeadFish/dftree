#include <stdio.h>
#include <assert.h>

//#include "dftree.h"
#include "rb.h"


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





void __thiscall
dftree_insert (dftree *rbtree, dfnode *node,
	int (*node_cmp)(const void *, const void*))
{
	struct {
		dfnode *node;
		int cmp;
	} path[sizeof(void *) << 4], *pathp;
	rbt_node_new(dfnode, link, rbtree, node);

	path->node = rbtree->rbt_root;
	for (pathp = path; pathp->node != NULL; pathp++) {
		int cmp = pathp->cmp = node_cmp(node, pathp->node);
		assert(cmp != 0);
		if (cmp < 0) {
			pathp[1].node = rbtn_left_get(dfnode, link, pathp->node);
		} else {
			pathp[1].node = rbtn_right_get(dfnode, link, pathp->node);
		}
	}
	pathp->node = node;

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
				return;
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
				return;
			}
		}
		pathp->node = cnode;
	}

	rbtree->rbt_root = path->node;
	rbtn_black_set(dfnode, link, rbtree->rbt_root);
};
