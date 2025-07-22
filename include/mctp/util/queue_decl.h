#ifndef _MCTP_UTIL_QUEUE_DECL_H_
#define _MCTP_UTIL_QUEUE_DECL_H_

#include <mctp/util/decl_common.h>
#include <mctp/util/extern_c.h>
#include <stdbool.h>


#define _queue_type_decl(typename, queue_value_t)   \
                                                    \
typedef struct _x_node_t(typename)                  \
{                                                   \
    queue_value_t data;                             \
    struct _x_node_t(typename) *next;               \
}                                                   \
_x_node_t(typename);                                \
                                                    \
typedef struct                                      \
{                                                   \
    _x_node_t(typename) *front;                     \
    _x_node_t(typename) *back;                      \
}                                                   \
_x_type_t(typename);                                \
                                                    \
                                                    \
EXTERN_C_BEGIN                                      \
                                                    \
_x_node_t(typename) *                               \
_x_method(typename, node_next)(                     \
    const _x_node_t(typename) *node                 \
);                                                  \
                                                    \
queue_value_t                                       \
_x_method(typename, node_data)(                     \
    const _x_node_t(typename) *node                 \
);                                                  \
                                                    \
_x_node_t(typename) *                               \
_x_method(typename, front)(                         \
    const _x_type_t(typename) *queue                \
);                                                  \
                                                    \
bool                                                \
_x_method(typename, empty)(                         \
    const _x_type_t(typename) *queue                \
);                                                  \
                                                    \
void                                                \
_x_method(typename, enqueue)(                       \
    _x_type_t(typename) *queue,                     \
    queue_value_t data                              \
);                                                  \
                                                    \
void                                                \
_x_method(typename, dequeue)(                       \
    _x_type_t(typename) *queue                      \
);                                                  \
                                                    \
void                                                \
_x_method(typename, clear)(                         \
    _x_type_t(typename) *queue                      \
);                                                  \
                                                    \
EXTERN_C_END

#endif // _MCTP_UTIL_QUEUE_DECL_H_
