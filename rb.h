

#define rb_node(a_type)							\
struct {								\
    a_type *rbn_left;							\
    intptr_t rbn_right_red;						\
    a_type* right_get() { return (a_type*) (rbn_right_red & -2LL); } \
    void right_set(a_type* x) { rbn_right_red = (rbn_right_red&1) | (intptr_t)x; } \
    intptr_t red_get() { return rbn_right_red & 1; } \
    void color_set(bool x) { rbn_right_red = (rbn_right_red&-2LL) | (intptr_t)x; } \
    void red_set() { rbn_right_red |= 1; } \
    void black_set() { rbn_right_red &= -2LL; } \
}

/* Root structure. */
#define rb_tree(a_type)							\
struct {								\
    a_type *rbt_root;							\
}

/* Left accessors. */
#define rbtn_left_get(a_type, a_field, a_node) ((a_node)->a_field.rbn_left)
#define rbtn_left_set(a_type, a_field, a_node, a_left) (a_node)->a_field.rbn_left = a_left;				\

/* Right accessors. */
#define rbtn_right_get(a_type, a_field, a_node) ((a_node)->a_field.right_get())
#define rbtn_right_set(a_type, a_field, a_node, a_right) (a_node)->a_field.right_set(a_right);

/* Color accessors. */
#define rbtn_red_get(a_type, a_field, a_node)	\
    (a_node)->a_field.red_get()
    
#define rbtn_color_set(a_type, a_field, a_node, a_red) (a_node)->a_field.color_set(a_red)						\

#define rbtn_red_set(a_type, a_field, a_node) (a_node)->a_field.red_set()
#define rbtn_black_set(a_type, a_field, a_node) (a_node)->a_field.black_set()

/* Node initializer. */
#define rbt_node_new(a_type, a_field, a_rbt, a_node) do {		\
    /* Bookkeeping bit cannot be used by node pointer. */		\
    assert(((uintptr_t)(a_node) & 0x1) == 0);				\
    rbtn_left_set(a_type, a_field, (a_node), NULL);	\
    rbtn_right_set(a_type, a_field, (a_node), NULL);	\
    rbtn_red_set(a_type, a_field, (a_node));				\
} while (0)
