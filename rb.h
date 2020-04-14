

#define rb_node(a_type)							\
struct {								\
    a_type *rbn_left;							\
    a_type *rbn_right_red;						\
}

/* Root structure. */
#define rb_tree(a_type)							\
struct {								\
    a_type *rbt_root;							\
}

/* Left accessors. */
#define rbtn_left_get(a_type, a_field, a_node)				\
    ((a_node)->a_field.rbn_left)
#define rbtn_left_set(a_type, a_field, a_node, a_left) do {		\
    (a_node)->a_field.rbn_left = a_left;				\
} while (0)

/* Right accessors. */
#define rbtn_right_get(a_type, a_field, a_node)				\
    ((a_type *) (((intptr_t) (a_node)->a_field.rbn_right_red)		\
      & ((ssize_t)-2)))
#define rbtn_right_set(a_type, a_field, a_node, a_right) do {		\
    (a_node)->a_field.rbn_right_red = (a_type *) (((uintptr_t) a_right)	\
      | (((uintptr_t) (a_node)->a_field.rbn_right_red) & ((size_t)1)));	\
} while (0)

/* Color accessors. */
#define rbtn_red_get(a_type, a_field, a_node)				\
    ((bool) (((uintptr_t) (a_node)->a_field.rbn_right_red)		\
      & ((size_t)1)))
#define rbtn_color_set(a_type, a_field, a_node, a_red) do {		\
    (a_node)->a_field.rbn_right_red = (a_type *) ((((intptr_t)		\
      (a_node)->a_field.rbn_right_red) & ((ssize_t)-2))			\
      | ((ssize_t)a_red));						\
} while (0)
#define rbtn_red_set(a_type, a_field, a_node) do {			\
    (a_node)->a_field.rbn_right_red = (a_type *) (((uintptr_t)		\
      (a_node)->a_field.rbn_right_red) | ((size_t)1));			\
} while (0)
#define rbtn_black_set(a_type, a_field, a_node) do {			\
    (a_node)->a_field.rbn_right_red = (a_type *) (((intptr_t)		\
      (a_node)->a_field.rbn_right_red) & ((ssize_t)-2));		\
} while (0)

/* Node initializer. */
#define rbt_node_new(a_type, a_field, a_rbt, a_node) do {		\
    /* Bookkeeping bit cannot be used by node pointer. */		\
    assert(((uintptr_t)(a_node) & 0x1) == 0);				\
    rbtn_left_set(a_type, a_field, (a_node), NULL);	\
    rbtn_right_set(a_type, a_field, (a_node), NULL);	\
    rbtn_red_set(a_type, a_field, (a_node));				\
} while (0)
