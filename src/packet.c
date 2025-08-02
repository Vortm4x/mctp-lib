#include <mctp/core/p_packet.h>
#include <mctp/util/alloc.h>


mctp_pkt_t *mctp_pkt_create(
    void
) {
    mctp_pkt_t *packet = zalloc(mctp_pkt_t);

    return packet;
}

void mctp_pkt_destroy(
    mctp_pkt_t **packet_ref
) {
    if (packet_ref == NULL) return;

    free(*packet_ref);
    *packet_ref = NULL;
}
