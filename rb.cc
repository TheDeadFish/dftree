
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




#define rb_gen(a_attr, a_prefix, a_field, a_cmp)	\
a_attr void								\
a_prefix##new(dftree *rbtree) {					\
    rbtree->rbt_root = NULL;				\
}									\
a_attr bool								\
a_prefix##empty(dftree *rbtree) {					\
    return (rbtree->rbt_root == NULL);					\
}									\
a_attr dfnode *								\
a_prefix##first(dftree *rbtree) {					\
    dfnode *ret;							\
    rbtn_first(dfnode, a_field, rbtree, rbtree->rbt_root, ret);		\
    return ret;								\
}									\
a_attr dfnode *								\
a_prefix##last(dftree *rbtree) {					\
    dfnode *ret;							\
    rbtn_last(dfnode, a_field, rbtree, rbtree->rbt_root, ret);		\
    return ret;								\
}									\
a_attr dfnode *								\
a_prefix##next(dftree *rbtree, dfnode *node) {			\
    dfnode *ret;							\
    if (rbtn_right_get(dfnode, a_field, node) != NULL) {		\
	rbtn_first(dfnode, a_field, rbtree, rbtn_right_get(dfnode,	\
	  a_field, node), ret);						\
    } else {								\
	dfnode *tnode = rbtree->rbt_root;				\
	assert(tnode != NULL);						\
	ret = NULL;							\
	while (true) {							\
	    int cmp = (a_cmp)(node, tnode);				\
	    if (cmp < 0) {						\
		ret = tnode;						\
		tnode = rbtn_left_get(dfnode, a_field, tnode);		\
	    } else if (cmp > 0) {					\
		tnode = rbtn_right_get(dfnode, a_field, tnode);		\
	    } else {							\
		break;							\
	    }								\
	    assert(tnode != NULL);					\
	}								\
    }									\
    return ret;								\
}									\
a_attr dfnode *								\
a_prefix##prev(dftree *rbtree, dfnode *node) {			\
    dfnode *ret;							\
    if (rbtn_left_get(dfnode, a_field, node) != NULL) {			\
	rbtn_last(dfnode, a_field, rbtree, rbtn_left_get(dfnode,	\
	  a_field, node), ret);						\
    } else {								\
	dfnode *tnode = rbtree->rbt_root;				\
	assert(tnode != NULL);						\
	ret = NULL;							\
	while (true) {							\
	    int cmp = (a_cmp)(node, tnode);				\
	    if (cmp < 0) {						\
		tnode = rbtn_left_get(dfnode, a_field, tnode);		\
	    } else if (cmp > 0) {					\
		ret = tnode;						\
		tnode = rbtn_right_get(dfnode, a_field, tnode);		\
	    } else {							\
		break;							\
	    }								\
	    assert(tnode != NULL);					\
	}								\
    }									\
    return ret;								\
}									\
a_attr dfnode *								\
a_prefix##search(dftree *rbtree, const dfnode *key) {		\
    dfnode *ret;							\
    int cmp;								\
    ret = rbtree->rbt_root;						\
    while (ret != NULL							\
      && (cmp = (a_cmp)(key, ret)) != 0) {				\
	if (cmp < 0) {							\
	    ret = rbtn_left_get(dfnode, a_field, ret);			\
	} else {							\
	    ret = rbtn_right_get(dfnode, a_field, ret);			\
	}								\
    }									\
    return ret;								\
}									\
a_attr dfnode *								\
a_prefix##nsearch(dftree *rbtree, const dfnode *key) {		\
    dfnode *ret;							\
    dfnode *tnode = rbtree->rbt_root;					\
    ret = NULL;								\
    while (tnode != NULL) {						\
	int cmp = (a_cmp)(key, tnode);					\
	if (cmp < 0) {							\
	    ret = tnode;						\
	    tnode = rbtn_left_get(dfnode, a_field, tnode);		\
	} else if (cmp > 0) {						\
	    tnode = rbtn_right_get(dfnode, a_field, tnode);		\
	} else {							\
	    ret = tnode;						\
	    break;							\
	}								\
    }									\
    return ret;								\
}									\
a_attr dfnode *								\
a_prefix##psearch(dftree *rbtree, const dfnode *key) {		\
    dfnode *ret;							\
    dfnode *tnode = rbtree->rbt_root;					\
    ret = NULL;								\
    while (tnode != NULL) {						\
	int cmp = (a_cmp)(key, tnode);					\
	if (cmp < 0) {							\
	    tnode = rbtn_left_get(dfnode, a_field, tnode);		\
	} else if (cmp > 0) {						\
	    ret = tnode;						\
	    tnode = rbtn_right_get(dfnode, a_field, tnode);		\
	} else {							\
	    ret = tnode;						\
	    break;							\
	}								\
    }									\
    return ret;								\
}									\
a_attr void								\
a_prefix##insert(dftree *rbtree, dfnode *node) {			\
    struct {								\
	dfnode *node;							\
	int cmp;							\
    } path[sizeof(void *) << 4], *pathp;				\
    rbt_node_new(dfnode, a_field, rbtree, node);			\
    /* Wind. */								\
    path->node = rbtree->rbt_root;					\
    for (pathp = path; pathp->node != NULL; pathp++) {			\
	int cmp = pathp->cmp = a_cmp(node, pathp->node);		\
	assert(cmp != 0);						\
	if (cmp < 0) {							\
	    pathp[1].node = rbtn_left_get(dfnode, a_field,		\
	      pathp->node);						\
	} else {							\
	    pathp[1].node = rbtn_right_get(dfnode, a_field,		\
	      pathp->node);						\
	}								\
    }									\
    pathp->node = node;							\
    /* Unwind. */							\
    for (pathp--; (uintptr_t)pathp >= (uintptr_t)path; pathp--) {	\
	dfnode *cnode = pathp->node;					\
	if (pathp->cmp < 0) {						\
	    dfnode *left = pathp[1].node;				\
	    rbtn_left_set(dfnode, a_field, cnode, left);		\
	    if (rbtn_red_get(dfnode, a_field, left)) {			\
		dfnode *leftleft = rbtn_left_get(dfnode, a_field, left);\
		if (leftleft != NULL && rbtn_red_get(dfnode, a_field,	\
		  leftleft)) {						\
		    /* Fix up 4-node. */				\
		    dfnode *tnode;					\
		    rbtn_black_set(dfnode, a_field, leftleft);		\
		    rbtn_rotate_right(dfnode, a_field, cnode, tnode);	\
		    cnode = tnode;					\
		}							\
	    } else {							\
		return;							\
	    }								\
	} else {							\
	    dfnode *right = pathp[1].node;				\
	    rbtn_right_set(dfnode, a_field, cnode, right);		\
	    if (rbtn_red_get(dfnode, a_field, right)) {			\
		dfnode *left = rbtn_left_get(dfnode, a_field, cnode);	\
		if (left != NULL && rbtn_red_get(dfnode, a_field,	\
		  left)) {						\
		    /* Split 4-node. */					\
		    rbtn_black_set(dfnode, a_field, left);		\
		    rbtn_black_set(dfnode, a_field, right);		\
		    rbtn_red_set(dfnode, a_field, cnode);		\
		} else {						\
		    /* Lean left. */					\
		    dfnode *tnode;					\
		    bool tred = rbtn_red_get(dfnode, a_field, cnode);	\
		    rbtn_rotate_left(dfnode, a_field, cnode, tnode);	\
		    rbtn_color_set(dfnode, a_field, tnode, tred);	\
		    rbtn_red_set(dfnode, a_field, cnode);		\
		    cnode = tnode;					\
		}							\
	    } else {							\
		return;							\
	    }								\
	}								\
	pathp->node = cnode;						\
    }									\
    /* Set root, and make it black. */					\
    rbtree->rbt_root = path->node;					\
    rbtn_black_set(dfnode, a_field, rbtree->rbt_root);			\
}									\
a_attr void								\
a_prefix##remove(dftree *rbtree, dfnode *node) {			\
    struct {								\
	dfnode *node;							\
	int cmp;							\
    } *pathp, *nodep, path[sizeof(void *) << 4];			\
    /* Wind. */								\
    nodep = NULL; /* Silence compiler warning. */			\
    path->node = rbtree->rbt_root;					\
    for (pathp = path; pathp->node != NULL; pathp++) {			\
	int cmp = pathp->cmp = a_cmp(node, pathp->node);		\
	if (cmp < 0) {							\
	    pathp[1].node = rbtn_left_get(dfnode, a_field,		\
	      pathp->node);						\
	} else {							\
	    pathp[1].node = rbtn_right_get(dfnode, a_field,		\
	      pathp->node);						\
	    if (cmp == 0) {						\
	        /* Find node's successor, in preparation for swap. */	\
		pathp->cmp = 1;						\
		nodep = pathp;						\
		for (pathp++; pathp->node != NULL; pathp++) {		\
		    pathp->cmp = -1;					\
		    pathp[1].node = rbtn_left_get(dfnode, a_field,	\
		      pathp->node);					\
		}							\
		break;							\
	    }								\
	}								\
    }									\
    assert(nodep->node == node);					\
    pathp--;								\
    if (pathp->node != node) {						\
	/* Swap node with its successor. */				\
	bool tred = rbtn_red_get(dfnode, a_field, pathp->node);		\
	rbtn_color_set(dfnode, a_field, pathp->node,			\
	  rbtn_red_get(dfnode, a_field, node));				\
	rbtn_left_set(dfnode, a_field, pathp->node,			\
	  rbtn_left_get(dfnode, a_field, node));			\
	/* If node's successor is its right child, the following code */\
	/* will do the wrong thing for the right child pointer.       */\
	/* However, it doesn't matter, because the pointer will be    */\
	/* properly set when the successor is pruned.                 */\
	rbtn_right_set(dfnode, a_field, pathp->node,			\
	  rbtn_right_get(dfnode, a_field, node));			\
	rbtn_color_set(dfnode, a_field, node, tred);			\
	/* The pruned leaf node's child pointers are never accessed   */\
	/* again, so don't bother setting them to nil.                */\
	nodep->node = pathp->node;					\
	pathp->node = node;						\
	if (nodep == path) {						\
	    rbtree->rbt_root = nodep->node;				\
	} else {							\
	    if (nodep[-1].cmp < 0) {					\
		rbtn_left_set(dfnode, a_field, nodep[-1].node,		\
		  nodep->node);						\
	    } else {							\
		rbtn_right_set(dfnode, a_field, nodep[-1].node,		\
		  nodep->node);						\
	    }								\
	}								\
    } else {								\
	dfnode *left = rbtn_left_get(dfnode, a_field, node);		\
	if (left != NULL) {						\
	    /* node has no successor, but it has a left child.        */\
	    /* Splice node out, without losing the left child.        */\
	    assert(!rbtn_red_get(dfnode, a_field, node));		\
	    assert(rbtn_red_get(dfnode, a_field, left));		\
	    rbtn_black_set(dfnode, a_field, left);			\
	    if (pathp == path) {					\
		rbtree->rbt_root = left;				\
	    } else {							\
		if (pathp[-1].cmp < 0) {				\
		    rbtn_left_set(dfnode, a_field, pathp[-1].node,	\
		      left);						\
		} else {						\
		    rbtn_right_set(dfnode, a_field, pathp[-1].node,	\
		      left);						\
		}							\
	    }								\
	    return;							\
	} else if (pathp == path) {					\
	    /* The tree only contained one node. */			\
	    rbtree->rbt_root = NULL;					\
	    return;							\
	}								\
    }									\
    if (rbtn_red_get(dfnode, a_field, pathp->node)) {			\
	/* Prune red node, which requires no fixup. */			\
	assert(pathp[-1].cmp < 0);					\
	rbtn_left_set(dfnode, a_field, pathp[-1].node, NULL);		\
	return;								\
    }									\
    /* The node to be pruned is black, so unwind until balance is     */\
    /* restored.                                                      */\
    pathp->node = NULL;							\
    for (pathp--; (uintptr_t)pathp >= (uintptr_t)path; pathp--) {	\
	assert(pathp->cmp != 0);					\
	if (pathp->cmp < 0) {						\
	    rbtn_left_set(dfnode, a_field, pathp->node,			\
	      pathp[1].node);						\
	    if (rbtn_red_get(dfnode, a_field, pathp->node)) {		\
		dfnode *right = rbtn_right_get(dfnode, a_field,		\
		  pathp->node);						\
		dfnode *rightleft = rbtn_left_get(dfnode, a_field,	\
		  right);						\
		dfnode *tnode;						\
		if (rightleft != NULL && rbtn_red_get(dfnode, a_field,	\
		  rightleft)) {						\
		    /* In the following diagrams, ||, //, and \\      */\
		    /* indicate the path to the removed node.         */\
		    /*                                                */\
		    /*      ||                                        */\
		    /*    pathp(r)                                    */\
		    /*  //        \                                   */\
		    /* (b)        (b)                                 */\
		    /*           /                                    */\
		    /*          (r)                                   */\
		    /*                                                */\
		    rbtn_black_set(dfnode, a_field, pathp->node);	\
		    rbtn_rotate_right(dfnode, a_field, right, tnode);	\
		    rbtn_right_set(dfnode, a_field, pathp->node, tnode);\
		    rbtn_rotate_left(dfnode, a_field, pathp->node,	\
		      tnode);						\
		} else {						\
		    /*      ||                                        */\
		    /*    pathp(r)                                    */\
		    /*  //        \                                   */\
		    /* (b)        (b)                                 */\
		    /*           /                                    */\
		    /*          (b)                                   */\
		    /*                                                */\
		    rbtn_rotate_left(dfnode, a_field, pathp->node,	\
		      tnode);						\
		}							\
		/* Balance restored, but rotation modified subtree    */\
		/* root.                                              */\
		assert((uintptr_t)pathp > (uintptr_t)path);		\
		if (pathp[-1].cmp < 0) {				\
		    rbtn_left_set(dfnode, a_field, pathp[-1].node,	\
		      tnode);						\
		} else {						\
		    rbtn_right_set(dfnode, a_field, pathp[-1].node,	\
		      tnode);						\
		}							\
		return;							\
	    } else {							\
		dfnode *right = rbtn_right_get(dfnode, a_field,		\
		  pathp->node);						\
		dfnode *rightleft = rbtn_left_get(dfnode, a_field,	\
		  right);						\
		if (rightleft != NULL && rbtn_red_get(dfnode, a_field,	\
		  rightleft)) {						\
		    /*      ||                                        */\
		    /*    pathp(b)                                    */\
		    /*  //        \                                   */\
		    /* (b)        (b)                                 */\
		    /*           /                                    */\
		    /*          (r)                                   */\
		    dfnode *tnode;					\
		    rbtn_black_set(dfnode, a_field, rightleft);		\
		    rbtn_rotate_right(dfnode, a_field, right, tnode);	\
		    rbtn_right_set(dfnode, a_field, pathp->node, tnode);\
		    rbtn_rotate_left(dfnode, a_field, pathp->node,	\
		      tnode);						\
		    /* Balance restored, but rotation modified        */\
		    /* subtree root, which may actually be the tree   */\
		    /* root.                                          */\
		    if (pathp == path) {				\
			/* Set root. */					\
			rbtree->rbt_root = tnode;			\
		    } else {						\
			if (pathp[-1].cmp < 0) {			\
			    rbtn_left_set(dfnode, a_field,		\
			      pathp[-1].node, tnode);			\
			} else {					\
			    rbtn_right_set(dfnode, a_field,		\
			      pathp[-1].node, tnode);			\
			}						\
		    }							\
		    return;						\
		} else {						\
		    /*      ||                                        */\
		    /*    pathp(b)                                    */\
		    /*  //        \                                   */\
		    /* (b)        (b)                                 */\
		    /*           /                                    */\
		    /*          (b)                                   */\
		    dfnode *tnode;					\
		    rbtn_red_set(dfnode, a_field, pathp->node);		\
		    rbtn_rotate_left(dfnode, a_field, pathp->node,	\
		      tnode);						\
		    pathp->node = tnode;				\
		}							\
	    }								\
	} else {							\
	    dfnode *left;						\
	    rbtn_right_set(dfnode, a_field, pathp->node,		\
	      pathp[1].node);						\
	    left = rbtn_left_get(dfnode, a_field, pathp->node);		\
	    if (rbtn_red_get(dfnode, a_field, left)) {			\
		dfnode *tnode;						\
		dfnode *leftright = rbtn_right_get(dfnode, a_field,	\
		  left);						\
		dfnode *leftrightleft = rbtn_left_get(dfnode, a_field,	\
		  leftright);						\
		if (leftrightleft != NULL && rbtn_red_get(dfnode,	\
		  a_field, leftrightleft)) {				\
		    /*      ||                                        */\
		    /*    pathp(b)                                    */\
		    /*   /        \\                                  */\
		    /* (r)        (b)                                 */\
		    /*   \                                            */\
		    /*   (b)                                          */\
		    /*   /                                            */\
		    /* (r)                                            */\
		    dfnode *unode;					\
		    rbtn_black_set(dfnode, a_field, leftrightleft);	\
		    rbtn_rotate_right(dfnode, a_field, pathp->node,	\
		      unode);						\
		    rbtn_rotate_right(dfnode, a_field, pathp->node,	\
		      tnode);						\
		    rbtn_right_set(dfnode, a_field, unode, tnode);	\
		    rbtn_rotate_left(dfnode, a_field, unode, tnode);	\
		} else {						\
		    /*      ||                                        */\
		    /*    pathp(b)                                    */\
		    /*   /        \\                                  */\
		    /* (r)        (b)                                 */\
		    /*   \                                            */\
		    /*   (b)                                          */\
		    /*   /                                            */\
		    /* (b)                                            */\
		    assert(leftright != NULL);				\
		    rbtn_red_set(dfnode, a_field, leftright);		\
		    rbtn_rotate_right(dfnode, a_field, pathp->node,	\
		      tnode);						\
		    rbtn_black_set(dfnode, a_field, tnode);		\
		}							\
		/* Balance restored, but rotation modified subtree    */\
		/* root, which may actually be the tree root.         */\
		if (pathp == path) {					\
		    /* Set root. */					\
		    rbtree->rbt_root = tnode;				\
		} else {						\
		    if (pathp[-1].cmp < 0) {				\
			rbtn_left_set(dfnode, a_field, pathp[-1].node,	\
			  tnode);					\
		    } else {						\
			rbtn_right_set(dfnode, a_field, pathp[-1].node,	\
			  tnode);					\
		    }							\
		}							\
		return;							\
	    } else if (rbtn_red_get(dfnode, a_field, pathp->node)) {	\
		dfnode *leftleft = rbtn_left_get(dfnode, a_field, left);\
		if (leftleft != NULL && rbtn_red_get(dfnode, a_field,	\
		  leftleft)) {						\
		    /*        ||                                      */\
		    /*      pathp(r)                                  */\
		    /*     /        \\                                */\
		    /*   (b)        (b)                               */\
		    /*   /                                            */\
		    /* (r)                                            */\
		    dfnode *tnode;					\
		    rbtn_black_set(dfnode, a_field, pathp->node);	\
		    rbtn_red_set(dfnode, a_field, left);		\
		    rbtn_black_set(dfnode, a_field, leftleft);		\
		    rbtn_rotate_right(dfnode, a_field, pathp->node,	\
		      tnode);						\
		    /* Balance restored, but rotation modified        */\
		    /* subtree root.                                  */\
		    assert((uintptr_t)pathp > (uintptr_t)path);		\
		    if (pathp[-1].cmp < 0) {				\
			rbtn_left_set(dfnode, a_field, pathp[-1].node,	\
			  tnode);					\
		    } else {						\
			rbtn_right_set(dfnode, a_field, pathp[-1].node,	\
			  tnode);					\
		    }							\
		    return;						\
		} else {						\
		    /*        ||                                      */\
		    /*      pathp(r)                                  */\
		    /*     /        \\                                */\
		    /*   (b)        (b)                               */\
		    /*   /                                            */\
		    /* (b)                                            */\
		    rbtn_red_set(dfnode, a_field, left);		\
		    rbtn_black_set(dfnode, a_field, pathp->node);	\
		    /* Balance restored. */				\
		    return;						\
		}							\
	    } else {							\
		dfnode *leftleft = rbtn_left_get(dfnode, a_field, left);\
		if (leftleft != NULL && rbtn_red_get(dfnode, a_field,	\
		  leftleft)) {						\
		    /*               ||                               */\
		    /*             pathp(b)                           */\
		    /*            /        \\                         */\
		    /*          (b)        (b)                        */\
		    /*          /                                     */\
		    /*        (r)                                     */\
		    dfnode *tnode;					\
		    rbtn_black_set(dfnode, a_field, leftleft);		\
		    rbtn_rotate_right(dfnode, a_field, pathp->node,	\
		      tnode);						\
		    /* Balance restored, but rotation modified        */\
		    /* subtree root, which may actually be the tree   */\
		    /* root.                                          */\
		    if (pathp == path) {				\
			/* Set root. */					\
			rbtree->rbt_root = tnode;			\
		    } else {						\
			if (pathp[-1].cmp < 0) {			\
			    rbtn_left_set(dfnode, a_field,		\
			      pathp[-1].node, tnode);			\
			} else {					\
			    rbtn_right_set(dfnode, a_field,		\
			      pathp[-1].node, tnode);			\
			}						\
		    }							\
		    return;						\
		} else {						\
		    /*               ||                               */\
		    /*             pathp(b)                           */\
		    /*            /        \\                         */\
		    /*          (b)        (b)                        */\
		    /*          /                                     */\
		    /*        (b)                                     */\
		    rbtn_red_set(dfnode, a_field, left);		\
		}							\
	    }								\
	}								\
    }									\
    /* Set root. */							\
    rbtree->rbt_root = path->node;					\
    assert(!rbtn_red_get(dfnode, a_field, rbtree->rbt_root));		\
}									\
a_attr dfnode *								\
a_prefix##iter_recurse(dftree *rbtree, dfnode *node,		\
  dfnode *(*cb)(dftree *, dfnode *, void *), void *arg) {		\
    if (node == NULL) {							\
	return NULL;							\
    } else {								\
	dfnode *ret;							\
	if ((ret = a_prefix##iter_recurse(rbtree, rbtn_left_get(dfnode,	\
	  a_field, node), cb, arg)) != NULL || (ret = cb(rbtree, node,	\
	  arg)) != NULL) {						\
	    return ret;							\
	}								\
	return a_prefix##iter_recurse(rbtree, rbtn_right_get(dfnode,	\
	  a_field, node), cb, arg);					\
    }									\
}									\
a_attr dfnode *								\
a_prefix##iter_start(dftree *rbtree, dfnode *start, dfnode *node,	\
  dfnode *(*cb)(dftree *, dfnode *, void *), void *arg) {		\
    int cmp = a_cmp(start, node);					\
    if (cmp < 0) {							\
	dfnode *ret;							\
	if ((ret = a_prefix##iter_start(rbtree, start,			\
	  rbtn_left_get(dfnode, a_field, node), cb, arg)) != NULL ||	\
	  (ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##iter_recurse(rbtree, rbtn_right_get(dfnode,	\
	  a_field, node), cb, arg);					\
    } else if (cmp > 0) {						\
	return a_prefix##iter_start(rbtree, start,			\
	  rbtn_right_get(dfnode, a_field, node), cb, arg);		\
    } else {								\
	dfnode *ret;							\
	if ((ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##iter_recurse(rbtree, rbtn_right_get(dfnode,	\
	  a_field, node), cb, arg);					\
    }									\
}									\
a_attr dfnode *								\
a_prefix##iter(dftree *rbtree, dfnode *start, dfnode *(*cb)(	\
  dftree *, dfnode *, void *), void *arg) {				\
    dfnode *ret;							\
    if (start != NULL) {						\
	ret = a_prefix##iter_start(rbtree, start, rbtree->rbt_root,	\
	  cb, arg);							\
    } else {								\
	ret = a_prefix##iter_recurse(rbtree, rbtree->rbt_root, cb, arg);\
    }									\
    return ret;								\
}									\
a_attr dfnode *								\
a_prefix##reverse_iter_recurse(dftree *rbtree, dfnode *node,	\
  dfnode *(*cb)(dftree *, dfnode *, void *), void *arg) {		\
    if (node == NULL) {							\
	return NULL;							\
    } else {								\
	dfnode *ret;							\
	if ((ret = a_prefix##reverse_iter_recurse(rbtree,		\
	  rbtn_right_get(dfnode, a_field, node), cb, arg)) != NULL ||	\
	  (ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##reverse_iter_recurse(rbtree,			\
	  rbtn_left_get(dfnode, a_field, node), cb, arg);		\
    }									\
}									\
a_attr dfnode *								\
a_prefix##reverse_iter_start(dftree *rbtree, dfnode *start,		\
  dfnode *node, dfnode *(*cb)(dftree *, dfnode *, void *),		\
  void *arg) {								\
    int cmp = a_cmp(start, node);					\
    if (cmp > 0) {							\
	dfnode *ret;							\
	if ((ret = a_prefix##reverse_iter_start(rbtree, start,		\
	  rbtn_right_get(dfnode, a_field, node), cb, arg)) != NULL ||	\
	  (ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##reverse_iter_recurse(rbtree,			\
	  rbtn_left_get(dfnode, a_field, node), cb, arg);		\
    } else if (cmp < 0) {						\
	return a_prefix##reverse_iter_start(rbtree, start,		\
	  rbtn_left_get(dfnode, a_field, node), cb, arg);		\
    } else {								\
	dfnode *ret;							\
	if ((ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##reverse_iter_recurse(rbtree,			\
	  rbtn_left_get(dfnode, a_field, node), cb, arg);		\
    }									\
}									\
a_attr dfnode *								\
a_prefix##reverse_iter(dftree *rbtree, dfnode *start,		\
  dfnode *(*cb)(dftree *, dfnode *, void *), void *arg) {		\
    dfnode *ret;							\
    if (start != NULL) {						\
	ret = a_prefix##reverse_iter_start(rbtree, start,		\
	  rbtree->rbt_root, cb, arg);					\
    } else {								\
	ret = a_prefix##reverse_iter_recurse(rbtree, rbtree->rbt_root,	\
	  cb, arg);							\
    }									\
    return ret;								\
}									\
a_attr void								\
a_prefix##destroy_recurse(dftree *rbtree, dfnode *node, void (*cb)(	\
  dfnode *, void *), void *arg) {					\
    if (node == NULL) {							\
	return;								\
    }									\
    a_prefix##destroy_recurse(rbtree, rbtn_left_get(dfnode, a_field,	\
      node), cb, arg);							\
    rbtn_left_set(dfnode, a_field, (node), NULL);			\
    a_prefix##destroy_recurse(rbtree, rbtn_right_get(dfnode, a_field,	\
      node), cb, arg);							\
    rbtn_right_set(dfnode, a_field, (node), NULL);			\
    if (cb) {								\
	cb(node, arg);							\
    }									\
}									\
a_attr void								\
a_prefix##destroy(dftree *rbtree, void (*cb)(dfnode *, void *),	\
  void *arg) {								\
    a_prefix##destroy_recurse(rbtree, rbtree->rbt_root, cb, arg);	\
    rbtree->rbt_root = NULL;						\
}

rb_gen(static, tree_, link, node_cmp);
