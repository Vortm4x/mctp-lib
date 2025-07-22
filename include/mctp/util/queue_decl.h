#ifndef _MCTP_UTIL_QUEUE_DECL_H_
#define _MCTP_UTIL_QUEUE_DECL_H_

#include <mctp/util/decl_common.h>
#include <mctp/util/extern_c.h>
#include <stdbool.h>


#define _queue_type_decl_private(   \
    typename,                       \
    queue_type_t,                   \
    queue_node_t,                   \
    queue_value_t                   \
)                                   \
                                    \
typedef struct queue_node_t         \
{                                   \
    queue_value_t data;             \
    struct queue_node_t *next;      \
}                                   \
queue_node_t;                       \
                                    \
typedef struct                      \
{                                   \
    queue_node_t *front;            \
    queue_node_t *back;             \
}                                   \
queue_type_t;                       \
                                    \
                                    \
EXTERN_C_BEGIN                      \
                                    \
queue_node_t *                      \
_x_method(typename, node_next)(     \
    const queue_node_t *node        \
);                                  \
                                    \
queue_value_t                       \
_x_method(typename, node_data)(     \
    const queue_node_t *node        \
);                                  \
                                    \
queue_node_t *                      \
_x_method(typename, front)(         \
    const queue_type_t *queue       \
);                                  \
                                    \
bool                                \
_x_method(typename, empty)(         \
    const queue_type_t *queue       \
);                                  \
                                    \
void                                \
_x_method(typename, enqueue)(       \
    queue_type_t *queue,            \
    queue_value_t data              \
);                                  \
                                    \
void                                \
_x_method(typename, dequeue)(       \
    queue_type_t *queue             \
);                                  \
                                    \
void                                \
_x_method(typename, clear)(         \
    queue_type_t *queue             \
);                                  \
                                    \
EXTERN_C_END


#define _queue_type_decl(typename, _x_value_t)  \
    _queue_type_decl_private(                   \
        typename,                               \
        _x_type_t(typename),                    \
        _x_node_t(typename),                    \
        _x_value_t                              \
    )

#endif // _MCTP_UTIL_QUEUE_DECL_H_
