#ifndef _MCTP_UTIL_AVL_MAP_IMPL_H_
#define _MCTP_UTIL_AVL_MAP_IMPL_H_

#include <mctp/util/typename.h>
#include <mctp/util/alloc.h>
#include <string.h>


#define _x_avl_map_value_iface_init(    \
    value_destroy,                      \
    value_gt,                           \
    value_lt,                           \
    value_eq                            \
) {                                     \
    .destroy    = value_destroy,        \
    .gt         = value_gt,             \
    .lt         = value_lt,             \
    .eq         = value_eq              \
};

#define _x_avl_map_value_iface_private( \
    value_iface,                        \
    avl_value_t,                        \
    avl_key_t                           \
) \
static const struct                                 \
{                                                   \
    void (*destroy)(avl_value_t *);                 \
    bool (*gt)(const avl_key_t, const avl_key_t);   \
    bool (*lt)(const avl_key_t, const avl_key_t);   \
    bool (*eq)(const avl_key_t, const avl_key_t);   \
}                                                   \
value_iface = _x_avl_map_value_iface_init

#define _x_avl_map_value_iface( \
    typename,                   \
    _x_value_t,                 \
    _x_key_t                    \
)                               \
_x_avl_map_value_iface_private( \
    _x_value_iface(typename),   \
    _x_value_t,                 \
    _x_key_t                    \
)


#define _x_avl_map_type_impl_private(   \
    avl_type_t,                         \
    avl_node_t,                         \
    avl_value_t,                        \
    avl_key_t,                          \
    avl_node_data,                      \
    avl_add,                            \
    avl_remove,                         \
    avl_get,                            \
    value_iface                         \
)                                       \
\
static avl_node_t *avl_node_create(         \
    avl_value_t data,                       \
    avl_key_t key                           \
) {                                         \
    avl_node_t *node = zalloc(avl_node_t);  \
    node->data = data;                      \
    node->key = key;                        \
    node->height = 1;                       \
                                            \
    return node;                            \
}                                           \
\
static void avl_node_destroy(           \
    avl_node_t *node                    \
) {                                     \
    if (node == NULL)                   \
    {                                   \
        return;                         \
    }                                   \
                                        \
    avl_node_destroy(node->left);       \
    avl_node_destroy(node->right);      \
    value_iface.destroy(&node->data);   \
                                        \
    free(node);                         \
}                                       \
\
static avl_height_t avl_height( \
    const avl_node_t *node      \
) {                             \
    if (node == NULL)           \
    {                           \
        return 0;               \
    }                           \
                                \
    return node->height;        \
}                               \
\
static void avl_update_height(                              \
    avl_node_t *node                                        \
) {                                                         \
    if (avl_height(node->left) > avl_height(node->right))   \
    {                                                       \
        node->height = 1 + avl_height(node->left);          \
    }                                                       \
    else                                                    \
    {                                                       \
        node->height = 1 + avl_height(node->right);         \
    }                                                       \
}                                                           \
\
static avl_height_t avl_balance(                                \
    const avl_node_t *node                                      \
) {                                                             \
    if (node == NULL)                                           \
    {                                                           \
        return 0;                                               \
    }                                                           \
                                                                \
    return avl_height(node->left) - avl_height(node->right);    \
}                                                               \
\
/*------------------------------|           \
//    old            new        |           \
//      \              \        |           \
//      new    =>      old      |           \
//      /                \      |           \
//    [x]                [x]    |           \
//-----------------------------*/           \
static avl_node_t *avl_rotate_left(         \
    avl_node_t *old_root                    \
) {                                         \
    avl_node_t *new_root = old_root->right; \
                                            \
    old_root->right = new_root->left;       \
    new_root->left = old_root;              \
                                            \
    avl_update_height(old_root);            \
    avl_update_height(new_root);            \
                                            \
    return new_root;                        \
}                                           \
\
/*------------------------------|           \
//      old              new    |           \
//      /                /      |           \
//    new      =>      old      |           \
//      \              /        |           \
//      [x]          [x]        |           \
//-----------------------------*/           \
static avl_node_t *avl_rotate_right(        \
    avl_node_t *old_root                    \
) {                                         \
    avl_node_t *new_root = old_root->left;  \
                                            \
    old_root->left = new_root->right;       \
    new_root->right = old_root;             \
                                            \
    avl_update_height(old_root);            \
    avl_update_height(new_root);            \
                                            \
    return new_root;                        \
}                                           \
\
static avl_node_t *avl_find_min(    \
    avl_node_t *node                \
) {                                 \
    while(node->left != NULL)       \
    {                               \
        node = node->left;          \
    }                               \
                                    \
    return node;                    \
}                                   \
\
avl_value_t avl_node_data(                      \
    const avl_node_t *node                      \
) {                                             \
    if (node == NULL)                           \
    {                                           \
        avl_value_t value;                      \
        memset(&value, 0, sizeof(avl_value_t)); \
                                                \
        return value;                           \
    }                                           \
                                                \
    return node->data;                          \
}                                               \
\
avl_node_t *avl_add(                                                        \
    avl_node_t *node,                                                       \
    avl_value_t data,                                                       \
    avl_key_t key                                                           \
) {                                                                         \
    if (node == NULL)                                                       \
    {                                                                       \
        return avl_node_create(data, key);                                  \
    }                                                                       \
    else if (value_iface.lt(key, node->key))                                \
    {                                                                       \
        node->left = avl_add(node->left, data, key);                        \
    }                                                                       \
    else if (value_iface.gt(key, node->key))                                \
    {                                                                       \
        node->right = avl_add(node->right, data, key);                      \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        return node;                                                        \
    }                                                                       \
                                                                            \
    avl_update_height(node);                                                \
                                                                            \
    if ((avl_balance(node) > 1) && !value_iface.eq(key, node->left->key))   \
    {                                                                       \
        if (value_iface.gt(key, node->left->key))                           \
        {                                                                   \
            node->left = avl_rotate_left(node->left);                       \
        }                                                                   \
                                                                            \
        return avl_rotate_right(node);                                      \
    }                                                                       \
                                                                            \
    if ((avl_balance(node) < -1) && !value_iface.eq(key, node->right->key)) \
    {                                                                       \
        if (value_iface.lt(key, node->right->key))                          \
        {                                                                   \
            node->right = avl_rotate_right(node->right);                    \
        }                                                                   \
                                                                            \
        return avl_rotate_left(node);                                       \
    }                                                                       \
                                                                            \
    return node;                                                            \
}                                                                           \
\
avl_node_t *avl_remove(                                         \
    avl_node_t *node,                                           \
    avl_key_t key                                               \
) {                                                             \
    if (node == NULL)                                           \
    {                                                           \
        return NULL;                                            \
    }                                                           \
    else if (value_iface.lt(key, node->key))                    \
    {                                                           \
        node->left = avl_remove(node->left, key);               \
    }                                                           \
    else if (value_iface.gt(key, node->key))                    \
    {                                                           \
        node->right = avl_remove(node->right, key);             \
    }                                                           \
    else                                                        \
    {                                                           \
        if ((node->left == NULL) && (node->right == NULL))      \
        {                                                       \
            avl_node_destroy(node);                             \
            node = NULL;                                        \
        }                                                       \
        else if (node->left == NULL)                            \
        {                                                       \
            avl_node_t *temp = node;                            \
            node = node->right;                                 \
            avl_node_destroy(temp);                             \
        }                                                       \
        else if (node->right == NULL)                           \
        {                                                       \
            avl_node_t *temp = node;                            \
            node = node->left;                                  \
            avl_node_destroy(temp);                             \
        }                                                       \
        else                                                    \
        {                                                       \
            avl_node_t *temp = avl_find_min(node->right);       \
            node->data = temp->data;                            \
            node->key = temp->key;                              \
            node->right = avl_remove(node->right, temp->key);   \
        }                                                       \
    }                                                           \
                                                                \
    if (node == NULL)                                           \
    {                                                           \
        return NULL;                                            \
    }                                                           \
                                                                \
    if (avl_balance(node) > 1)                                  \
    {                                                           \
        if (avl_balance(node->left) < 0)                        \
        {                                                       \
            node->left = avl_rotate_left(node->left);           \
        }                                                       \
                                                                \
        return avl_rotate_right(node);                          \
    }                                                           \
                                                                \
    if (avl_balance(node) < -1)                                 \
    {                                                           \
        if (avl_balance(node->right) > 0)                       \
        {                                                       \
            node->right = avl_rotate_right(node->right);        \
        }                                                       \
                                                                \
        return avl_rotate_left(node);                           \
    }                                                           \
                                                                \
    return node;                                                \
}                                                               \
\
avl_node_t *avl_get(                        \
    avl_node_t *node,                       \
    avl_key_t key                           \
) {                                         \
    if (node == NULL)                       \
    {                                       \
        return NULL;                        \
    }                                       \
                                            \
    if (value_iface.lt(key, node->key))     \
    {                                       \
        return avl_get(node->left, key);    \
    }                                       \
                                            \
    if (value_iface.gt(key, node->key))     \
    {                                       \
        return avl_get(node->right, key);   \
    }                                       \
                                            \
    return node;                            \
}


#define _x_avl_map_type_impl(   \
    typename,                   \
    _x_value_t,                 \
    _x_key_t                    \
)                               \
_x_avl_map_type_impl_private(       \
    _x_type_t(typename),            \
    _x_node_t(typename),            \
    _x_value_t,                     \
    _x_key_t,                       \
    _x_method(typename, node_data), \
    _x_method(typename, add),       \
    _x_method(typename, remove),    \
    _x_method(typename, get),       \
    _x_value_iface(typename)        \
)

#endif // _MCTP_UTIL_AVL_MAP_IMPL_H_
