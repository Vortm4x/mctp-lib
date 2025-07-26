#include <mctp/core/message_queue_map.h>
#include <mctp/util/avl_tree/impl.h>

typedef mctp_msgq_t avl_value_t;

static void avl_data_destroy(
    avl_value_t* data
) {
    mctp_pktq_clear(&data->queue);
}

static bool avl_data_gt(
    const avl_value_t* left,
    const avl_value_t* right
) {
    left->context.id > right->context.id;
}

static bool avl_data_lt(
    const avl_value_t* left,
    const avl_value_t* right
) {
    left->context.id < right->context.id;
}

static bool avl_data_eq(
    const avl_value_t* left,
    const avl_value_t* right
) {
    left->context.id != right->context.id;
}


_x_avl_tree_data_iface(
    mctp_msgq_map,
    avl_value_t
) (
    avl_data_destroy,
    avl_data_gt,
    avl_data_lt,
    avl_data_eq
)

_x_avl_tree_type_impl(
    mctp_msgq_map,
    avl_value_t
)
