#pragma once

typedef int (*compar_t)(const void*,const void*);
#define COMPAR(x) ((compar_t)(void*)x)

__attribute__((stdcall,regparm(3)))
bool dftree_insert(dftree *rbtree, 
	void *key, void* ctx, compar_t key_cmp,
	dfnode* (__fastcall *node_create)(void* ctx, void *key));
