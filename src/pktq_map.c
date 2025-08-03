#include <mctp/util/avl_map/impl.h>
#include <mctp/core/p_pktq_map.h>
#include <mctp/core/packet_queue.h>

typedef mctp_pktq_t *avl_value_t;
typedef mctp_msg_ctx_raw_t avl_key_t;
    

static void avl_value_destroy(
    avl_value_t *pkt_queue_ref
) {
    if (pkt_queue_ref == NULL) return;

    mctp_pktq_destroy(*pkt_queue_ref);
}

_x_avl_map_value_iface(
    mctp_pktq_map,
    avl_value_t,
    avl_key_t
) (
    avl_value_destroy
)

_x_avl_map_type_impl(
    mctp_pktq_map,
    avl_value_t,
    avl_key_t
)
