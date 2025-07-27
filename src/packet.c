#include <mctp/core/packet.h>
#include <mctp/util/alloc.h>
#include <stdlib.h>
#include <string.h>

mctp_packet_t *mctp_pkt_create(
    const mctp_transport_header_t *header,
    const uint8_t payload_data[],
    const size_t payload_len
) {
    mctp_packet_t *packet = zalloc(mctp_packet_t);

    memcpy(&packet->io.header, header, sizeof(mctp_transport_header_t));
    memcpy(packet->io.payload, payload_data, payload_len);
    packet->len = payload_len + sizeof(mctp_transport_header_t);

    return packet;
}

mctp_packet_t *mctp_pkt_clone(
    const mctp_packet_t *packet
) {
    mctp_packet_t *duplicate = zalloc(mctp_packet_t);

    memcpy(duplicate, packet, sizeof(mctp_packet_t));

    return duplicate;
}

void mctp_pkt_destroy(
    mctp_packet_t *packet
) {
    free(packet);
}

size_t mctp_pkt_payload_len(
    const mctp_packet_t *packet
) {
    if (packet->len < MCTP_PKT_HDR_SIZE) {
        return 0;
    }

    return (packet->len - MCTP_PKT_HDR_SIZE);
}
