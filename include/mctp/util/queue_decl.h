#ifndef _MCTP_UTIL_QUEUE_DECL_H_
#define _MCTP_UTIL_QUEUE_DECL_H_

#define _queue_t(q_x) q_x##_t
#define _queue_node_t(q_x) q_x##_node_t
#define _queue_method(q_x, method) q_x##_##method

#define _queue_type_decl(q_x, value_t)                      \
                                                            \
    typedef struct _queue_node_t(q_x)                       \
    {                                                       \
        value_t value;                                      \
        struct _queue_node_t(q_x) *next;                    \
    }                                                       \
    _queue_node_t(q_x);                                     \
                                                            \
    typedef struct                                          \
    {                                                       \
        _queue_node_t(q_x) *front;                          \
        _queue_node_t(q_x) *rear;                           \
    }                                                       \
    _queue_t(q_x);                                          \
                                                            \
    _queue_node_t(q_x) *_queue_method(q_x, node_create)(    \
        value_t value                                       \
    );                                                      \
                                                            \
    void _queue_method(q_x, node_destroy)(                  \
        _queue_node_t(q_x) *node                            \
    );                                                      \
                                                            \
    _queue_node_t(q_x) *_queue_method(q_x, node_next)(      \
        const _queue_node_t(q_x) *node                      \
    );                                                      \
                                                            \
    value_t _queue_method(q_x, node_value)(                 \
        const _queue_node_t(q_x) *node                      \
    );                                                      \
                                                            \
    void _queue_method(q_x, enqueue)(                       \
        _queue_t(q_x) *queue,                               \
        value_t value                                       \
    );                                                      \
                                                            \
    value_t _queue_method(q_x, dequeue)(                    \
        _queue_t(q_x) *queue                                \
    );                                                      \
                                                            \
    _queue_node_t(q_x) *_queue_method(q_x, front)(          \
        const _queue_t(q_x) *queue                          \
    );                                                      \
                                                            \
    bool _queue_method(q_x, empty)(                         \
        const _queue_t(q_x) *queue                          \
    );                                                      \
                                                            \
    _queue_node_t(q_x) *_queue_method(q_x, clear)(          \
        _queue_t(q_x) *queue                                \
    );

#endif // _MCTP_UTIL_QUEUE_DECL_H_
