#pragma once

struct dfnode {	
  dfnode *left_ptr;	
  intptr_t right_red;	
	
	// pointer set/get
	dfnode* left() { return left_ptr; }
	dfnode* right() { return (dfnode*)(right_red & -2LL); }
	void left_set(dfnode* p) { left_ptr = p; }
  void right_set(dfnode* p) { right_red = (right_red&1) | (intptr_t)p; }
	
	// color set/get
	intptr_t color() { return right_red & 1; } 
	void color_set(intptr_t c) { right_red = (right_red&-2LL) | (intptr_t)c; }
  void red_set() { right_red |= 1; } void black_set() { right_red &= -2LL; }	

  void init() { left_ptr = 0; right_red = 1; }
	void set_right_red(dfnode* p, intptr_t c) { right_red = (intptr_t)p | c; }
};

struct dftree { dfnode *rbt_root;	};


typedef int (*compar_t)(const void*,const void*);
#define COMPAR(x) ((compar_t)(void*)x)

__attribute__((stdcall,regparm(3)))
bool dftree_insert(dftree *rbtree, 
	void *key, void* ctx, compar_t key_cmp,
	dfnode* (__fastcall *node_create)(void* ctx, void *key));
