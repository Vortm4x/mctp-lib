#include <mctp/core/queue_ctx_map.h>


#define typename mctp_qctx_map
#define avl_value_t mctp_msgq_t


#define avl_data_destroy(data) \
    mctp_pktq_clear(&data.queue)

#define avl_key(data) \
    (data.context.id)

#define avl_data_gt(left, right) \
    (avl_key(left) > avl_key(right))

#define avl_data_lt(left, right) \
    (avl_key(left) < avl_key(right))

#define avl_data_eq(left, right) \
    (avl_key(left) == avl_key(right))


#include <mctp/util/avl_tree_impl.h>
