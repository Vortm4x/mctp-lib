#ifndef _MCTP_UTIL_QUEUE_IMPL_H_
#define _MCTP_UTIL_QUEUE_IMPL_H_

#include <mctp/util/alloc.h>


#define _queue_t(q_x) q_x##_t
#define _queue_node_t(q_x) q_x##_node_t
#define _queue_method(q_x, method) q_x##_##method
#define _queue_value_deinit_stub(value) while (false)

#define _queue_type_impl(q_x, value_t, value_deinit, value_default)         \
                                                                            \
    _queue_node_t(q_x) *_queue_method(q_x, node_create)(                    \
        value_t value                                                       \
    ) {                                                                     \
        _queue_node_t(q_x) *node = _alloc(_queue_node_t(q_x));              \
                                                                            \
        node->value = value;                                                \
                                                                            \
        return node;                                                        \
    }                                                                       \
                                                                            \
    void _queue_method(q_x, node_destroy)(                                  \
        _queue_node_t(q_x) *node                                            \
    ) {                                                                     \
        free(node);                                                         \
    }                                                                       \
                                                                            \
    _queue_node_t(q_x) *_queue_method(q_x, node_next)(                      \
        const _queue_node_t(q_x) *node                                      \
    ) {                                                                     \
        if (node == NULL)                                                   \
        {                                                                   \
            return NULL;                                                    \
        }                                                                   \
                                                                            \
        return node->next;                                                  \
    }                                                                       \
                                                                            \
    value_t _queue_method(q_x, node_value)(                                 \
        const _queue_node_t(q_x) *node                                      \
    ) {                                                                     \
        return node->value;                                                 \
    }                                                                       \
                                                                            \
    void _queue_method(q_x, enqueue)(                                       \
        _queue_t(q_x) *queue,                                               \
        value_t value                                                       \
    ) {                                                                     \
        _queue_node_t(q_x) *node = _queue_method(q_x, node_create)(value);  \
                                                                            \
        if (_queue_method(q_x, empty)(queue))                               \
        {                                                                   \
            queue->front = node;                                            \
        }                                                                   \
        else                                                                \
        {                                                                   \
            queue->rear->next = node;                                       \
        }                                                                   \
                                                                            \
        queue->rear = node;                                                 \
    }                                                                       \
                                                                            \
    value_t _queue_method(q_x, dequeue)(                                    \
        _queue_t(q_x) *queue                                                \
    ) {                                                                     \
        value_t value = value_default;                                      \
                                                                            \
        if (!_queue_method(q_x, empty)(queue))                              \
        {                                                                   \
            _queue_node_t(q_x) *node = queue->front;                        \
            value = node->value;                                            \
                                                                            \
            queue->front = queue->front->next;                              \
                                                                            \
            if (_queue_method(q_x, empty)(queue))                           \
            {                                                               \
                queue->rear = NULL;                                         \
            }                                                               \
                                                                            \
            _queue_method(q_x, node_destroy)(node);                         \
        }                                                                   \
                                                                            \
        return value;                                                       \
    }                                                                       \
                                                                            \
    _queue_node_t(q_x) *_queue_method(q_x, front)(                          \
        const _queue_t(q_x) *queue                                          \
    ) {                                                                     \
        return queue->front;                                                \
    }                                                                       \
                                                                            \
    bool _queue_method(q_x, empty)(                                         \
        const _queue_t(q_x) *queue                                          \
    ) {                                                                     \
        return (queue->front == NULL);                                      \
    }                                                                       \
                                                                            \
    void _queue_method(q_x, clear)(                                         \
        _queue_t(q_x) *queue                                                \
    ) {                                                                     \
        while(!_queue_method(q_x, empty)(queue))                            \
        {                                                                   \
            value_t value = _queue_method(q_x, dequeue)(queue);             \
            value_deinit(value);                                            \
        }                                                                   \
    }

#endif // _MCTP_UTIL_QUEUE_IMPL_H_
