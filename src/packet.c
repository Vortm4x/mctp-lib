#include <mctp/core/p_packet.h>
#include <mctp/util/alloc.h>
#include <string.h>


mctp_pkt_t *mctp_pkt_create(
    void
) {
    mctp_pkt_t *packet = zalloc(mctp_pkt_t);

    return packet;
}

mctp_pkt_t *mctp_pkt_clone(
    const mctp_pkt_t *packet
) {
    if (packet == NULL) return NULL;

    mctp_pkt_t *copy = mctp_pkt_create();

    memcpy(copy->io.data, packet->io.data, packet->len);
    copy->len = packet->len;

    return copy;
}

void mctp_pkt_destroy(
    mctp_pkt_t *packet
) {
    if (packet == NULL) return;

    free(packet);
}
