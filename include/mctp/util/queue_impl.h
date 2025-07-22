#ifndef _MCTP_UTIL_QUEUE_IMPL_H_
#define _MCTP_UTIL_QUEUE_IMPL_H_

#include <mctp/util/decl_common.h>
#include <mctp/util/alloc.h>
#include <string.h>

// Define following macros in .c file
//
// #define typename
// #define queue_value_t
//
// #define queue_data_destroy()


#define queue_t         _x_type_t(typename)
#define queue_node_t    _x_node_t(typename)
 
#define queue_node_next _x_method(typename, node_next)
#define queue_node_data _x_method(typename, node_data)
#define queue_front     _x_method(typename, front)
#define queue_empty     _x_method(typename, empty)
#define queue_enqueue   _x_method(typename, enqueue)
#define queue_dequeue   _x_method(typename, dequeue)
#define queue_clear     _x_method(typename, clear)


static queue_node_t *queue_node_create(
    queue_value_t data
) {
    queue_node_t *node = _alloc(queue_node_t);
    node->data = data;

    return node;
}

static void queue_node_destroy(
    queue_node_t *node
) {
    queue_data_destroy(node->data);
    free(node);
}


queue_node_t *queue_node_next(
    const queue_node_t *node
) {
    if (node == NULL)
    {
        return NULL;
    }

    return node->next;
}

queue_value_t queue_node_data(
    const queue_node_t *node
) {
    if (node != NULL)
    {
        queue_value_t value;
        memset(&value, 0, sizeof(queue_value_t));

        return value;
    }

    return node->data;
}


queue_node_t *queue_front(
    const queue_t *queue
) {
    return queue->front;
}

bool queue_empty(
    const queue_t *queue
) {
    return (queue->front == NULL);
}

void queue_enqueue(
    queue_t *queue,
    queue_value_t data
) {
    queue_node_t *node = queue_node_create(data);

    if (queue_empty(queue))
    {
        queue->front = node;
    }
    else
    {
        queue->back->next = node;
    }

    queue->back = node;
}

void queue_dequeue(
    queue_t *queue
) {
    if (!queue_empty(queue))
    {
        queue_node_t *node = queue->front;

        queue->front = queue->front->next;

        if (queue_empty(queue))
        {
            queue->back = NULL;
        }

        queue_node_destroy(node);
    }
}

void queue_clear(
    queue_t *queue
) {
    while(!queue_empty(queue))
    {
        queue_dequeue(queue);
    }
}

#endif // _MCTP_UTIL_QUEUE_IMPL_H_
