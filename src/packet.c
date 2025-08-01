#include <mctp/core/packet_p.h>
#include <mctp/util/alloc.h>


mctp_pkt_t *mctp_pkt_create(
    void
) {
    mctp_pkt_t *packet = zalloc(mctp_pkt_t);

    return packet;
}

void mctp_pkt_destroy(
    mctp_pkt_t *packet
) {
    free(packet);
}
