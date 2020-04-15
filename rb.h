

struct dfnode {	
  dfnode *rbn_left;	
  intptr_t rbn_right_red;	
  
  dfnode* right_get() { return (dfnode*) (rbn_right_red & -2LL); }
  void right_set(dfnode* x) { rbn_right_red = (rbn_right_red&1) | (intptr_t)x; }
  intptr_t red_get() { return rbn_right_red & 1; } 
  void color_set(bool x) { rbn_right_red = (rbn_right_red&-2LL) | (intptr_t)x; }
  void red_set() { rbn_right_red |= 1; }
  void black_set() { rbn_right_red &= -2LL; }
  
  void init() { rbn_left = 0; rbn_right_red = 1; }
};

struct dftree { dfnode *rbt_root;	};


/* Left accessors. */
#define rbtn_left_get(a_type, a_field, a_node) ((a_node)->rbn_left)
#define rbtn_left_set(a_type, a_field, a_node, a_left) (a_node)->rbn_left = a_left;				\

/* Right accessors. */
#define rbtn_right_get(a_type, a_field, a_node) ((a_node)->right_get())
#define rbtn_right_set(a_type, a_field, a_node, a_right) (a_node)->right_set(a_right);

/* Color accessors. */
#define rbtn_red_get(a_type, a_field, a_node)	\
    (a_node)->red_get()
    
#define rbtn_color_set(a_type, a_field, a_node, a_red) (a_node)->color_set(a_red)						\

#define rbtn_red_set(a_type, a_field, a_node) (a_node)->red_set()
#define rbtn_black_set(a_type, a_field, a_node) (a_node)->black_set()

/* Node initializer. */
#define rbt_node_new(a_type, a_field, a_rbt, a_node) do {		\
    /* Bookkeeping bit cannot be used by node pointer. */		\
    assert(((uintptr_t)(a_node) & 0x1) == 0);				\
    rbtn_left_set(a_type, a_field, (a_node), NULL);	\
    rbtn_right_set(a_type, a_field, (a_node), NULL);	\
    rbtn_red_set(a_type, a_field, (a_node));				\
} while (0)
