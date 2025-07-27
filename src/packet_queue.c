#include <mctp/core/packet_queue.h>
#include <mctp/util/queue/impl.h>

typedef mctp_packet_t* queue_value_t;

static void queue_value_destroy(
    queue_value_t* packet
) {
    mctp_pkt_destroy(*packet);
}


_x_queue_value_iface(
    mctp_pktq,
    queue_value_t
) (
    queue_value_destroy
)

_x_queue_type_impl(
    mctp_pktq,
    queue_value_t
)
