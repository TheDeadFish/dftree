#pragma once

typedef int (*compar_t)(const void*,const void*);
#define COMPAR(x) ((compar_t)(void*)x)



__thiscall void 
dftree_insert (dftree *rbtree, dfnode *node,
	int (*node_cmp)(const void *, const void*));
