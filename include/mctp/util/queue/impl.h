#ifndef _MCTP_UTIL_QUEUE_IMPL_H_
#define _MCTP_UTIL_QUEUE_IMPL_H_

#include <mctp/util/typename.h>
#include <mctp/util/alloc.h>
#include <string.h>


#define _x_queue_data_iface(queue_value_t)  \
static const struct                         \
{                                           \
    void (*destroy)(queue_value_t *);       \
}                                           \
data_iface =


#define _x_queue_type_impl_private( \
    queue_type_t,                   \
    queue_node_t,                   \
    queue_value_t,                  \
    queue_node_next,                \
    queue_node_data,                \
    queue_front,                    \
    queue_empty,                    \
    queue_enqueue,                  \
    queue_dequeue,                  \
    queue_clear                     \
)                                   \
\
static queue_node_t *queue_node_create(         \
    queue_value_t data                          \
) {                                             \
    queue_node_t *node = _alloc(queue_node_t);  \
    node->data = data;                          \
                                                \
    return node;                                \
}                                               \
\
static void queue_node_destroy(                 \
    queue_node_t *node                          \
) {                                             \
    queue_data_destroy(&node->data);            \
    free(node);                                 \
}                                               \
\
queue_node_t *queue_node_next(  \
    const queue_node_t *node    \
) {                             \
    if (node == NULL)           \
    {                           \
        return NULL;            \
    }                           \
                                \
    return node->next;          \
}                               \
\
queue_value_t queue_node_data(                      \
    const queue_node_t *node                        \
) {                                                 \
    if (node == NULL)                               \
    {                                               \
        queue_value_t value;                        \
        memset(&value, 0, sizeof(queue_value_t));   \
                                                    \
        return value;                               \
    }                                               \
                                                    \
    return node->data;                              \
}                                                   \
\
queue_node_t *queue_front(      \
    const queue_type_t *queue   \
) {                             \
    return queue->front;        \
}                               \
\
bool queue_empty(                   \
    const queue_type_t *queue       \
) {                                 \
    return (queue->front == NULL);  \
}                                   \
\
void queue_enqueue(                                 \
    queue_type_t *queue,                            \
    queue_value_t data                              \
) {                                                 \
    queue_node_t *node = queue_node_create(data);   \
                                                    \
    if (queue_empty(queue))                         \
    {                                               \
        queue->front = node;                        \
    }                                               \
    else                                            \
    {                                               \
        queue->back->next = node;                   \
    }                                               \
                                                    \
    queue->back = node;                             \
}                                                   \
\
void queue_dequeue(                         \
    queue_type_t *queue                     \
) {                                         \
    if (!queue_empty(queue))                \
    {                                       \
        queue_node_t *node = queue->front;  \
                                            \
        queue->front = queue->front->next;  \
                                            \
        if (queue_empty(queue))             \
        {                                   \
            queue->back = NULL;             \
        }                                   \
                                            \
        queue_node_destroy(node);           \
    }                                       \
}                                           \
\
void queue_clear(               \
    queue_type_t *queue         \
) {                             \
    while(!queue_empty(queue))  \
    {                           \
        queue_dequeue(queue);   \
    }                           \
}


#define _x_queue_type_impl(typename, _x_value_t)    \
_x_queue_type_impl_private(                         \
    _x_type_t(typename),                            \
    _x_node_t(typename),                            \
    _x_value_t,                                     \
    _x_method(typename, node_next),                 \
    _x_method(typename, node_data),                 \
    _x_method(typename, front),                     \
    _x_method(typename, empty),                     \
    _x_method(typename, enqueue),                   \
    _x_method(typename, dequeue),                   \
    _x_method(typename, clear)                      \
)

#endif // _MCTP_UTIL_QUEUE_IMPL_H_
