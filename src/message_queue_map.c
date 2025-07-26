#include <mctp/core/message_queue_map.h>
#include <mctp/util/avl_tree/impl.h>

typedef mctp_msgq_t avl_value_t;

static void avl_value_destroy(
    avl_value_t* data
) {
    mctp_pktq_clear(&data->queue);
}

static bool avl_value_gt(
    const avl_value_t* left,
    const avl_value_t* right
) {
    left->context.id > right->context.id;
}

static bool avl_value_lt(
    const avl_value_t* left,
    const avl_value_t* right
) {
    left->context.id < right->context.id;
}

static bool avl_value_eq(
    const avl_value_t* left,
    const avl_value_t* right
) {
    left->context.id != right->context.id;
}


_x_avl_tree_value_iface(
    mctp_msgq_map,
    avl_value_t
) (
    avl_value_destroy,
    avl_value_gt,
    avl_value_lt,
    avl_value_eq
)

_x_avl_tree_type_impl(
    mctp_msgq_map,
    avl_value_t
)
