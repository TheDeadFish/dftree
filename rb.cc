
/* Internal utility macros. */
#define rbtn_first(a_rbt, a_root, r_node) do {		\
    (r_node) = (a_root);						\
    if ((r_node) != NULL) {						\
	for (;								\
	  rbtn_left_get((r_node)) != NULL;		\
	  (r_node) = rbtn_left_get((r_node))) {	\
	}								\
    }									\
} while (0)

#define rbtn_last(a_rbt, a_root, r_node) do {		\
    (r_node) = (a_root);						\
    if ((r_node) != NULL) {						\
	for (; rbtn_right_get((r_node)) != NULL;	\
	  (r_node) = rbtn_right_get((r_node))) {	\
	}								\
    }									\
} while (0)

#define rbtn_rotate_left(a_node, r_node) do {		\
    (r_node) = rbtn_right_get((a_node));		\
    rbtn_right_set((a_node),				\
      rbtn_left_get((r_node)));			\
    rbtn_left_set((r_node), (a_node));			\
} while (0)

#define rbtn_rotate_right(a_node, r_node) do {		\
    (r_node) = rbtn_left_get((a_node));		\
    rbtn_left_set((a_node),				\
      rbtn_right_get((r_node)));			\
    rbtn_right_set((r_node), (a_node));		\
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
    rbtn_first(rbtree, rbtree->rbt_root, ret);		\
    return ret;								\
}									\
a_attr dfnode *								\
a_prefix##last(dftree *rbtree) {					\
    dfnode *ret;							\
    rbtn_last(rbtree, rbtree->rbt_root, ret);		\
    return ret;								\
}									\
a_attr dfnode *								\
a_prefix##next(dftree *rbtree, dfnode *node) {			\
    dfnode *ret;							\
    if (rbtn_right_get(node) != NULL) {		\
	rbtn_first(rbtree, rbtn_right_get(node), ret);						\
    } else {								\
	dfnode *tnode = rbtree->rbt_root;				\
	assert(tnode != NULL);						\
	ret = NULL;							\
	while (true) {							\
	    int cmp = (a_cmp)(node, tnode);				\
	    if (cmp < 0) {						\
		ret = tnode;						\
		tnode = rbtn_left_get(tnode);		\
	    } else if (cmp > 0) {					\
		tnode = rbtn_right_get(tnode);		\
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
    if (rbtn_left_get(node) != NULL) {			\
	rbtn_last(rbtree, rbtn_left_get(node), ret);						\
    } else {								\
	dfnode *tnode = rbtree->rbt_root;				\
	assert(tnode != NULL);						\
	ret = NULL;							\
	while (true) {							\
	    int cmp = (a_cmp)(node, tnode);				\
	    if (cmp < 0) {						\
		tnode = rbtn_left_get(tnode);		\
	    } else if (cmp > 0) {					\
		ret = tnode;						\
		tnode = rbtn_right_get(tnode);		\
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
	    ret = rbtn_left_get(ret);			\
	} else {							\
	    ret = rbtn_right_get(ret);			\
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
	    tnode = rbtn_left_get(tnode);		\
	} else if (cmp > 0) {						\
	    tnode = rbtn_right_get(tnode);		\
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
	    tnode = rbtn_left_get(tnode);		\
	} else if (cmp > 0) {						\
	    ret = tnode;						\
	    tnode = rbtn_right_get(tnode);		\
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
    rbt_node_new(rbtree, node);			\
    /* Wind. */								\
    path->node = rbtree->rbt_root;					\
    for (pathp = path; pathp->node != NULL; pathp++) {			\
	int cmp = pathp->cmp = a_cmp(node, pathp->node);		\
	assert(cmp != 0);						\
	if (cmp < 0) {							\
	    pathp[1].node = rbtn_left_get(		\
	      pathp->node);						\
	} else {							\
	    pathp[1].node = rbtn_right_get(		\
	      pathp->node);						\
	}								\
    }									\
    pathp->node = node;							\
    /* Unwind. */							\
    for (pathp--; (uintptr_t)pathp >= (uintptr_t)path; pathp--) {	\
	dfnode *cnode = pathp->node;					\
	if (pathp->cmp < 0) {						\
	    dfnode *left = pathp[1].node;				\
	    rbtn_left_set(cnode, left);		\
	    if (rbtn_red_get(left)) {			\
		dfnode *leftleft = rbtn_left_get(left);\
		if (leftleft != NULL && rbtn_red_get(	\
		  leftleft)) {						\
		    /* Fix up 4-node. */				\
		    dfnode *tnode;					\
		    rbtn_black_set(leftleft);		\
		    rbtn_rotate_right(cnode, tnode);	\
		    cnode = tnode;					\
		}							\
	    } else {							\
		return;							\
	    }								\
	} else {							\
	    dfnode *right = pathp[1].node;				\
	    rbtn_right_set(cnode, right);		\
	    if (rbtn_red_get(right)) {			\
		dfnode *left = rbtn_left_get(cnode);	\
		if (left != NULL && rbtn_red_get(	\
		  left)) {						\
		    /* Split 4-node. */					\
		    rbtn_black_set(left);		\
		    rbtn_black_set(right);		\
		    rbtn_red_set(cnode);		\
		} else {						\
		    /* Lean left. */					\
		    dfnode *tnode;					\
		    bool tred = rbtn_red_get(cnode);	\
		    rbtn_rotate_left(cnode, tnode);	\
		    rbtn_color_set(tnode, tred);	\
		    rbtn_red_set(cnode);		\
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
    rbtn_black_set(rbtree->rbt_root);			\
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
	    pathp[1].node = rbtn_left_get(		\
	      pathp->node);						\
	} else {							\
	    pathp[1].node = rbtn_right_get(		\
	      pathp->node);						\
	    if (cmp == 0) {						\
	        /* Find node's successor, in preparation for swap. */	\
		pathp->cmp = 1;						\
		nodep = pathp;						\
		for (pathp++; pathp->node != NULL; pathp++) {		\
		    pathp->cmp = -1;					\
		    pathp[1].node = rbtn_left_get(	\
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
	bool tred = rbtn_red_get(pathp->node);		\
	rbtn_color_set(pathp->node,			\
	  rbtn_red_get(node));				\
	rbtn_left_set(pathp->node,			\
	  rbtn_left_get(node));			\
	/* If node's successor is its right child, the following code */\
	/* will do the wrong thing for the right child pointer.       */\
	/* However, it doesn't matter, because the pointer will be    */\
	/* properly set when the successor is pruned.                 */\
	rbtn_right_set(pathp->node,			\
	  rbtn_right_get(node));			\
	rbtn_color_set(node, tred);			\
	/* The pruned leaf node's child pointers are never accessed   */\
	/* again, so don't bother setting them to nil.                */\
	nodep->node = pathp->node;					\
	pathp->node = node;						\
	if (nodep == path) {						\
	    rbtree->rbt_root = nodep->node;				\
	} else {							\
	    if (nodep[-1].cmp < 0) {					\
		rbtn_left_set(nodep[-1].node,		\
		  nodep->node);						\
	    } else {							\
		rbtn_right_set(nodep[-1].node,		\
		  nodep->node);						\
	    }								\
	}								\
    } else {								\
	dfnode *left = rbtn_left_get(node);		\
	if (left != NULL) {						\
	    /* node has no successor, but it has a left child.        */\
	    /* Splice node out, without losing the left child.        */\
	    assert(!rbtn_red_get(node));		\
	    assert(rbtn_red_get(left));		\
	    rbtn_black_set(left);			\
	    if (pathp == path) {					\
		rbtree->rbt_root = left;				\
	    } else {							\
		if (pathp[-1].cmp < 0) {				\
		    rbtn_left_set(pathp[-1].node,	\
		      left);						\
		} else {						\
		    rbtn_right_set(pathp[-1].node,	\
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
    if (rbtn_red_get(pathp->node)) {			\
	/* Prune red node, which requires no fixup. */			\
	assert(pathp[-1].cmp < 0);					\
	rbtn_left_set(pathp[-1].node, NULL);		\
	return;								\
    }									\
    /* The node to be pruned is black, so unwind until balance is     */\
    /* restored.                                                      */\
    pathp->node = NULL;							\
    for (pathp--; (uintptr_t)pathp >= (uintptr_t)path; pathp--) {	\
	assert(pathp->cmp != 0);					\
	if (pathp->cmp < 0) {						\
	    rbtn_left_set(pathp->node,			\
	      pathp[1].node);						\
	    if (rbtn_red_get(pathp->node)) {		\
		dfnode *right = rbtn_right_get(		\
		  pathp->node);						\
		dfnode *rightleft = rbtn_left_get(	\
		  right);						\
		dfnode *tnode;						\
		if (rightleft != NULL && rbtn_red_get(	\
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
		    rbtn_black_set(pathp->node);	\
		    rbtn_rotate_right(right, tnode);	\
		    rbtn_right_set(pathp->node, tnode);\
		    rbtn_rotate_left(pathp->node,	\
		      tnode);						\
		} else {						\
		    /*      ||                                        */\
		    /*    pathp(r)                                    */\
		    /*  //        \                                   */\
		    /* (b)        (b)                                 */\
		    /*           /                                    */\
		    /*          (b)                                   */\
		    /*                                                */\
		    rbtn_rotate_left(pathp->node,	\
		      tnode);						\
		}							\
		/* Balance restored, but rotation modified subtree    */\
		/* root.                                              */\
		assert((uintptr_t)pathp > (uintptr_t)path);		\
		if (pathp[-1].cmp < 0) {				\
		    rbtn_left_set(pathp[-1].node,	\
		      tnode);						\
		} else {						\
		    rbtn_right_set(pathp[-1].node,	\
		      tnode);						\
		}							\
		return;							\
	    } else {							\
		dfnode *right = rbtn_right_get(		\
		  pathp->node);						\
		dfnode *rightleft = rbtn_left_get(	\
		  right);						\
		if (rightleft != NULL && rbtn_red_get(	\
		  rightleft)) {						\
		    /*      ||                                        */\
		    /*    pathp(b)                                    */\
		    /*  //        \                                   */\
		    /* (b)        (b)                                 */\
		    /*           /                                    */\
		    /*          (r)                                   */\
		    dfnode *tnode;					\
		    rbtn_black_set(rightleft);		\
		    rbtn_rotate_right(right, tnode);	\
		    rbtn_right_set(pathp->node, tnode);\
		    rbtn_rotate_left(pathp->node,	\
		      tnode);						\
		    /* Balance restored, but rotation modified        */\
		    /* subtree root, which may actually be the tree   */\
		    /* root.                                          */\
		    if (pathp == path) {				\
			/* Set root. */					\
			rbtree->rbt_root = tnode;			\
		    } else {						\
			if (pathp[-1].cmp < 0) {			\
			    rbtn_left_set(		\
			      pathp[-1].node, tnode);			\
			} else {					\
			    rbtn_right_set(		\
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
		    rbtn_red_set(pathp->node);		\
		    rbtn_rotate_left(pathp->node,	\
		      tnode);						\
		    pathp->node = tnode;				\
		}							\
	    }								\
	} else {							\
	    dfnode *left;						\
	    rbtn_right_set(pathp->node,		\
	      pathp[1].node);						\
	    left = rbtn_left_get(pathp->node);		\
	    if (rbtn_red_get(left)) {			\
		dfnode *tnode;						\
		dfnode *leftright = rbtn_right_get(	\
		  left);						\
		dfnode *leftrightleft = rbtn_left_get(	\
		  leftright);						\
		if (leftrightleft != NULL && rbtn_red_get(leftrightleft)) {				\
		    /*      ||                                        */\
		    /*    pathp(b)                                    */\
		    /*   /        \\                                  */\
		    /* (r)        (b)                                 */\
		    /*   \                                            */\
		    /*   (b)                                          */\
		    /*   /                                            */\
		    /* (r)                                            */\
		    dfnode *unode;					\
		    rbtn_black_set(leftrightleft);	\
		    rbtn_rotate_right(pathp->node,	\
		      unode);						\
		    rbtn_rotate_right(pathp->node,	\
		      tnode);						\
		    rbtn_right_set(unode, tnode);	\
		    rbtn_rotate_left(unode, tnode);	\
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
		    rbtn_red_set(leftright);		\
		    rbtn_rotate_right(pathp->node,	\
		      tnode);						\
		    rbtn_black_set(tnode);		\
		}							\
		/* Balance restored, but rotation modified subtree    */\
		/* root, which may actually be the tree root.         */\
		if (pathp == path) {					\
		    /* Set root. */					\
		    rbtree->rbt_root = tnode;				\
		} else {						\
		    if (pathp[-1].cmp < 0) {				\
			rbtn_left_set(pathp[-1].node,	\
			  tnode);					\
		    } else {						\
			rbtn_right_set(pathp[-1].node,	\
			  tnode);					\
		    }							\
		}							\
		return;							\
	    } else if (rbtn_red_get(pathp->node)) {	\
		dfnode *leftleft = rbtn_left_get(left);\
		if (leftleft != NULL && rbtn_red_get(	\
		  leftleft)) {						\
		    /*        ||                                      */\
		    /*      pathp(r)                                  */\
		    /*     /        \\                                */\
		    /*   (b)        (b)                               */\
		    /*   /                                            */\
		    /* (r)                                            */\
		    dfnode *tnode;					\
		    rbtn_black_set(pathp->node);	\
		    rbtn_red_set(left);		\
		    rbtn_black_set(leftleft);		\
		    rbtn_rotate_right(pathp->node,	\
		      tnode);						\
		    /* Balance restored, but rotation modified        */\
		    /* subtree root.                                  */\
		    assert((uintptr_t)pathp > (uintptr_t)path);		\
		    if (pathp[-1].cmp < 0) {				\
			rbtn_left_set(pathp[-1].node,	\
			  tnode);					\
		    } else {						\
			rbtn_right_set(pathp[-1].node,	\
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
		    rbtn_red_set(left);		\
		    rbtn_black_set(pathp->node);	\
		    /* Balance restored. */				\
		    return;						\
		}							\
	    } else {							\
		dfnode *leftleft = rbtn_left_get(left);\
		if (leftleft != NULL && rbtn_red_get(	\
		  leftleft)) {						\
		    /*               ||                               */\
		    /*             pathp(b)                           */\
		    /*            /        \\                         */\
		    /*          (b)        (b)                        */\
		    /*          /                                     */\
		    /*        (r)                                     */\
		    dfnode *tnode;					\
		    rbtn_black_set(leftleft);		\
		    rbtn_rotate_right(pathp->node,	\
		      tnode);						\
		    /* Balance restored, but rotation modified        */\
		    /* subtree root, which may actually be the tree   */\
		    /* root.                                          */\
		    if (pathp == path) {				\
			/* Set root. */					\
			rbtree->rbt_root = tnode;			\
		    } else {						\
			if (pathp[-1].cmp < 0) {			\
			    rbtn_left_set(		\
			      pathp[-1].node, tnode);			\
			} else {					\
			    rbtn_right_set(		\
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
		    rbtn_red_set(left);		\
		}							\
	    }								\
	}								\
    }									\
    /* Set root. */							\
    rbtree->rbt_root = path->node;					\
    assert(!rbtn_red_get(rbtree->rbt_root));		\
}									\
a_attr dfnode *								\
a_prefix##iter_recurse(dftree *rbtree, dfnode *node,		\
  dfnode *(*cb)(dftree *, dfnode *, void *), void *arg) {		\
    if (node == NULL) {							\
	return NULL;							\
    } else {								\
	dfnode *ret;							\
	if ((ret = a_prefix##iter_recurse(rbtree, rbtn_left_get(node), cb, arg)) != NULL || (ret = cb(rbtree, node,	\
	  arg)) != NULL) {						\
	    return ret;							\
	}								\
	return a_prefix##iter_recurse(rbtree, rbtn_right_get(node), cb, arg);					\
    }									\
}									\
a_attr dfnode *								\
a_prefix##iter_start(dftree *rbtree, dfnode *start, dfnode *node,	\
  dfnode *(*cb)(dftree *, dfnode *, void *), void *arg) {		\
    int cmp = a_cmp(start, node);					\
    if (cmp < 0) {							\
	dfnode *ret;							\
	if ((ret = a_prefix##iter_start(rbtree, start,			\
	  rbtn_left_get(node), cb, arg)) != NULL ||	\
	  (ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##iter_recurse(rbtree, rbtn_right_get(node), cb, arg);					\
    } else if (cmp > 0) {						\
	return a_prefix##iter_start(rbtree, start,			\
	  rbtn_right_get(node), cb, arg);		\
    } else {								\
	dfnode *ret;							\
	if ((ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##iter_recurse(rbtree, rbtn_right_get(node), cb, arg);					\
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
	  rbtn_right_get(node), cb, arg)) != NULL ||	\
	  (ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##reverse_iter_recurse(rbtree,			\
	  rbtn_left_get(node), cb, arg);		\
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
	  rbtn_right_get(node), cb, arg)) != NULL ||	\
	  (ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##reverse_iter_recurse(rbtree,			\
	  rbtn_left_get(node), cb, arg);		\
    } else if (cmp < 0) {						\
	return a_prefix##reverse_iter_start(rbtree, start,		\
	  rbtn_left_get(node), cb, arg);		\
    } else {								\
	dfnode *ret;							\
	if ((ret = cb(rbtree, node, arg)) != NULL) {			\
	    return ret;							\
	}								\
	return a_prefix##reverse_iter_recurse(rbtree,			\
	  rbtn_left_get(node), cb, arg);		\
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
    a_prefix##destroy_recurse(rbtree, rbtn_left_get(	\
      node), cb, arg);							\
    rbtn_left_set((node), NULL);			\
    a_prefix##destroy_recurse(rbtree, rbtn_right_get(	\
      node), cb, arg);							\
    rbtn_right_set((node), NULL);			\
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
