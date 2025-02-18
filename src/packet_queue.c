#include <mctp/core/packet_queue.h>
#include <mctp/util/queue_impl.h>

_queue_type_impl(
    mctp_pktq,
    mctp_packet_t *,
    mctp_pkt_destroy,
    NULL
)
