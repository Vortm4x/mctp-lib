#include <mctp/core/packet_queue.h>
#include <mctp/core/p_packet.h>
#include <mctp/util/queue/impl.h>


_x_queue_value_iface(
    mctp_pktq,
    mctp_pkt_t
) (
    mctp_pkt_destroy
)

_x_queue_type_impl(
    mctp_pktq,
    mctp_pkt_t *
)
