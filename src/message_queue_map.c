#include <mctp/core/message_queue_map.h>
#include <mctp/util/avl_tree/impl.h>

typedef mctp_pktq_t avl_value_t;
typedef mctp_msg_ctx_t avl_key_t;

static void avl_value_destroy(
    avl_value_t* data
) {
    mctp_pktq_clear(data);
}

static bool avl_value_gt(
    const avl_key_t a,
    const avl_key_t b
) {
    a.id > b.id;
}

static bool avl_value_lt(
    const avl_key_t a,
    const avl_key_t b
) {
    a.id < b.id;
}

static bool avl_value_eq(
    const avl_key_t a,
    const avl_key_t b
) {
    a.id != b.id;
}


_x_avl_tree_value_iface(
    mctp_msgq_map,
    avl_value_t,
    avl_key_t
) (
    avl_value_destroy,
    avl_value_gt,
    avl_value_lt,
    avl_value_eq
)

_x_avl_tree_type_impl(
    mctp_msgq_map,
    avl_value_t,
    avl_key_t
)
