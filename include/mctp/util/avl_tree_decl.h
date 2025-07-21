#ifndef _MCTP_UTIL_AVL_TREE_DECL_H_
#define _MCTP_UTIL_AVL_TREE_DECL_H_

#include <mctp/util/decl_common.h>
#include <mctp/util/extern_c.h>
#include <stdint.h>


typedef int8_t avl_height_t;


#define _avl_tree_type_decl(typename, avl_value_t)  \
                                                    \
typedef struct _x_node_t(typename)                  \
{                                                   \
    struct _x_node_t(typename) *left;               \
    struct _x_node_t(typename) *right;              \
    avl_height_t height;                            \
    avl_value_t data;                               \
}                                                   \
_x_node_t(typename);                                \
                                                    \
                                                    \
EXTERN_C_BEGIN                                      \
                                                    \
_x_node_t(typename) *                               \
_x_method(typename, add)(                           \
    _x_node_t(typename) *node,                      \
    avl_value_t data                                \
);                                                  \
                                                    \
_x_node_t(typename) *                               \
_x_method(typename, remove)(                        \
    _x_node_t(typename) *node,                      \
    avl_value_t data                                \
);                                                  \
                                                    \
_x_node_t(typename) *                               \
_x_method(typename, get)(                           \
    _x_node_t(typename) *node,                      \
    avl_value_t data                                \
);                                                  \
                                                    \
EXTERN_C_END


#endif // _MCTP_UTIL_AVL_TREE_DECL_H_
