#ifndef _MCTP_UTIL_AVL_TREE_DECL_H_
#define _MCTP_UTIL_AVL_TREE_DECL_H_

#include <mctp/util/decl_common.h>
#include <mctp/util/extern_c.h>
#include <stdint.h>

typedef int8_t avl_height_t;

#define _avl_tree_type_decl_private(    \
    typename,                           \
    avl_type_t,                         \
    avl_node_t,                         \
    avl_value_t                         \
)                                       \
                                        \
typedef struct avl_node_t               \
{                                       \
    struct avl_node_t *left;            \
    struct avl_node_t *right;           \
    avl_height_t height;                \
    avl_value_t data;                   \
}                                       \
avl_node_t;                             \
                                        \
typedef avl_node_t avl_type_t;          \
                                        \
                                        \
EXTERN_C_BEGIN                          \
                                        \
avl_value_t                             \
_x_method(typename, node_data)(         \
    avl_node_t *node                    \
);                                      \
                                        \
avl_node_t *                            \
_x_method(typename, add)(               \
    avl_node_t *node,                   \
    avl_value_t data                    \
);                                      \
                                        \
avl_node_t *                            \
_x_method(typename, remove)(            \
    avl_node_t *node,                   \
    avl_value_t data                    \
);                                      \
                                        \
avl_node_t *                            \
_x_method(typename, get)(               \
    avl_node_t *node,                   \
    avl_value_t data                    \
);                                      \
                                        \
EXTERN_C_END


#define _avl_tree_type_decl(typename, _x_value_t)   \
    _avl_tree_type_decl_private(                    \
        typename,                                   \
        _x_type_t(typename),                        \
        _x_node_t(typename),                        \
        _x_value_t                                  \
    )

#endif // _MCTP_UTIL_AVL_TREE_DECL_H_
