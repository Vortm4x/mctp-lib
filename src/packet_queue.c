#include <mctp/core/packet_queue.h>
#include <mctp/util/queue/impl.h>

typedef mctp_packet_t* queue_value_t;

static void queue_data_destroy(
    queue_value_t* data
) {
    mctp_pkt_destroy(*data);
}


_x_queue_data_iface(
    mctp_pktq,
    queue_value_t
) (
    queue_data_destroy
)

_x_queue_type_impl(
    mctp_pktq,
    queue_value_t
)
