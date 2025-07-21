#ifndef _MCTP_UTIL_AVL_TREE_IMPL_H_
#define _MCTP_UTIL_AVL_TREE_IMPL_H_

#include <mctp/util/decl_common.h>
#include <mctp/util/alloc.h>

// Define following macros in .c file
//
// #define typename
// #define avl_value_t
//
// #define avl_data_destroy(data)
// #define avl_data_gt(left, right)
// #define avl_data_lt(left, right)
// #define avl_data_eq(left, right)


#define avl_node_t      _x_node_t(typename)
#define avl_add         _x_method(typename, add)
#define avl_remove      _x_method(typename, remove)
#define avl_get         _x_method(typename, get)


static avl_node_t *avl_node_create(
    avl_value_t data
) {
    avl_node_t *node = _alloc(avl_node_t);
    node->data = data;
    node->height = 1;

    return node;
}

static void avl_node_destroy(
    avl_node_t *node
) {
    if (node == NULL)
    {
        return;
    }

    avl_node_destroy(node->left);
    avl_node_destroy(node->right);
    avl_data_destroy(node->data);
    
    free(node);
}

static avl_height_t avl_height(
    avl_node_t *node
) {
    if (node == NULL)
    {
        return 0;    
    }

    return node->height;
}


static void avl_update_height(
    avl_node_t *node
) {
    if (avl_height(node->left) > avl_height(node->right))
    {
        node->height = 1 + avl_height(node->left);
    }
    else
    {
        node->height = 1 + avl_height(node->right);
    }
}

static avl_height_t avl_balance(
    avl_node_t *node
) {
    if (node == NULL)
    {
        return 0;
    }

    return avl_height(node->left) - avl_height(node->right);
}

//------------------------------|
//    old            new        |
//      \              \        |
//      new    =>      old      |
//      /                \      |
//    [x]                [x]    |
//------------------------------|
static avl_node_t *avl_rotate_left(
    avl_node_t *old_root
)
{
    avl_node_t *new_root = old_root->right;

    old_root->right = new_root->left;
    new_root->left = old_root;

    avl_update_height(old_root);
    avl_update_height(new_root);

    return new_root;
}

//------------------------------|
//      old              new    |
//      /                /      |
//    new      =>      old      |
//      \              /        |
//      [x]          [x]        |
//------------------------------|
static avl_node_t *avl_rotate_right(
    avl_node_t *old_root
) {
    avl_node_t *new_root = old_root->left;

    old_root->left = new_root->right;
    new_root->right = old_root;

    avl_update_height(old_root);
    avl_update_height(new_root);

    return new_root;
}

static avl_node_t *avl_find_min(avl_node_t *node)
{
    while(node->left != NULL)
    {
        node = node->left;
    }

    return node;
}


avl_node_t *avl_add(
    avl_node_t *node,
    avl_value_t data
) {
    if (node == NULL)
    {
        return avl_node_create(data);
    }
    else if (avl_data_lt(data, node->data))
    {
        node->left = avl_add(node->left, data);
    }
    else if (avl_data_gt(data, node->data))
    {
        node->right = avl_add(node->right, data);
    }
    else
    {
        return node;
    }

    avl_update_height(node);

    if ((avl_balance(node) > 1) && !avl_data_eq(data, node->left->data))
    {
        if (avl_data_gt(data, node->left->data))
        {
            node->left = avl_rotate_left(node->left);
        }

        return avl_rotate_right(node);
    }

    if ((avl_balance(node) < -1) && !avl_data_eq(data, node->right->data) )
    {
        if (avl_data_lt(data, node->right->data))
        {
            node->right = avl_rotate_right(node->right);
        }

        return avl_rotate_left(node);
    }

    return node;
}

avl_node_t *avl_remove(
    avl_node_t *node,
    avl_value_t data
) {
    if (node == NULL)
    {
        return NULL;
    }
    else if (avl_data_lt(data, node->data))
    {
        node->left = avl_remove(node->left, data);
    }
    else if (avl_data_gt(data, node->data))
    {
        node->right = avl_remove(node->right, data);
    }
    else
    {
        if ((node->left == NULL) && (node->right == NULL)) 
        {
            avl_node_destroy(node);
            node = NULL;
        }
        else if (node->left == NULL)
        {
            avl_node_t *temp = node;
            node = node->right;
            avl_node_destroy(temp);
        }
        else if (node->right == NULL)
        {
            avl_node_t *temp = node;
            node = node->left;
            avl_node_destroy(temp);
        }
        else
        {
            avl_node_t *temp = avl_find_min(node->right);
            node->data = temp->data;
            node->right = avl_remove(node->right, temp->data);
        }
    }

    if (node == NULL)
    {
        return NULL;
    }

    if (avl_balance(node) > 1)
    {
        if (avl_balance(node->left) < 0)
        {
            node->left = avl_rotate_left(node->left);
        }

        return avl_rotate_right(node);
    }

    if (avl_balance(node) < -1)
    {
        if (avl_balance(node->right) > 0)
        {
            node->right = avl_rotate_right(node->right);
        }

        return avl_rotate_left(node);
    }

    return node;
}

avl_node_t *avl_get(
    avl_node_t *node,
    avl_value_t data
) {
    if (node == NULL)
    {
        return NULL;
    }

    if (avl_data_lt(data, node->data))
    {
        return avl_get(node->left, data);
    }

    if (avl_data_gt(data, node->data))
    {
        return avl_get(node->right, data);
    }

    return node;
}

#endif // _MCTP_UTIL_AVL_TREE_IMPL_H_
