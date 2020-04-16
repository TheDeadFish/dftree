#pragma once

typedef int (*compar_t)(const void*,const void*);
#define REGCALL(x) __attribute__((stdcall,regparm(x)))




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
REGCALL(3) dftree_insret_t dftree_insert(dftree *rbtree, void *key, 
	void* ctx, compar_t key_cmp, dfnode_create_t node_create);
	
REGCALL(3) void dftree_insert(dftree *rbtree, 
	dfnode *node, compar_t key_cmp);

// tree search 
REGCALL(3) dfnode* dftree_search(dfnode* node, 
	const void* key, compar_t key_cmp);
REGCALL(3) dfnode* dftree_nsearch(dfnode* node, 
	const void* key, compar_t key_cmp);
REGCALL(3) dfnode* dftree_psearch(dfnode* node, 
	const void* key, compar_t key_cmp);

// tree traversal
REGCALL(3) void* dftree_iter_recurse_(dfnode* node, void* ctx, dftree_iter_t cb);
REGCALL(3) void* dftree_iter_rrecurse_(dfnode* node, void* ctx, dftree_iter_t cb);


struct dftree_path
{
	struct list_t { dfnode *node; int cmp; };
	list_t* pathp; list_t path[sizeof(void*)*14];

	bool chk(list_t* x) { return (x > (list_t*)this); }
	
	void insert(dftree* tree, dfnode* inode);
	
	dfnode* init(dfnode* node, void* key, compar_t key_cmp);
};