#include <mctp/core/packet.h>
#include <stdlib.h>
#include <string.h>

mctp_packet_t *mctp_pkt_create(
    const mctp_transport_header_t *header,
    const uint8_t payload_data[],
    const size_t payload_len
) {
    mctp_packet_t *packet = (mctp_packet_t *)calloc(1, sizeof(mctp_packet_t));

    memcpy(&packet->header, header, sizeof(mctp_transport_header_t));
    memcpy(packet->payload, payload_data, payload_len);
    packet->len = payload_len + sizeof(mctp_transport_header_t);

    return packet;
}

void mctp_pkt_destroy(
    mctp_packet_t *packet
) {
    free(packet);
}
