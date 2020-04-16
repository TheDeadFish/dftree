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
	
	
	template <class T> auto* cont(T x) {
		return container_of(this, x); }
};

// function and types
struct dftree_insret_t { dfnode* node; bool inserted; };
typedef dfnode* (__fastcall *dfnode_create_t)(void* ctx, void *key);
typedef void* (__fastcall *dftree_iter_t)(void*, dfnode*);



struct dftree { dfnode *root;
	operator dfnode*() { return root; }
};

// tree modifcation
__attribute__((stdcall,regparm(3)))
dftree_insret_t dftree_insert(dftree *rbtree, void *key, 
	void* ctx, compar_t key_cmp, dfnode_create_t node_create);

// tree search 
__fastcall dfnode* dftree_search(dfnode* node, 
	const void* key, compar_t key_cmp);
__fastcall dfnode* dftree_nsearch(dfnode* node, 
	const void* key, compar_t key_cmp);
__fastcall dfnode* dftree_psearch(dfnode* node, 
	const void* key, compar_t key_cmp);

// tree traversal
__fastcall void* dftree_iter_recurse_(dfnode* node, void* ctx, dftree_iter_t cb);
__fastcall void* dftree_iter_rrecurse_(dfnode* node, void* ctx, dftree_iter_t cb);

