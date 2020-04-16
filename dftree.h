#pragma once

typedef int (*compar_t)(const void*,const void*);

// this will go somewhere else later
template <class T>
struct TaggedPtr1
{
	// construction / assignment
	TaggedPtr1() {}
	TaggedPtr1(T* ptr, intptr_t tag=0) : 
		data(intptr_t(ptr) | tag) {}
	void operator=(T* p) { ptr_set(p); }

	// pointer access
	T* operator->() {return ptr();}
	T& operator*() {return *ptr();}
	operator T*() {return ptr();}
	
	// pointer getters
	T* ptr() { return (T*)(data & -2); }
	intptr_t tag() { return data & 1; }
	void ptr_set(T* ptr) { data = (data&1) | intptr_t(ptr); }
	void tag_set(intptr_t tag) { data = (data&-2) | tag; }
	intptr_t data;	
};


struct dfnode {	
  dfnode *left;	
  TaggedPtr1<dfnode> right;	
	
	// color set/get
	intptr_t color() { return right.tag(); } 
	void color_set(intptr_t c) { right.tag_set(c); }
  void red_set() { right.tag_set(1); } 
	void black_set() { right.tag_set(0); }	

  void init() { left = 0; right = {0,1}; }
	void set_right(dfnode* p, intptr_t c) { right = {p, c}; }
	
	
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

