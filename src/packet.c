#include <mctp/core/packet.h>
#include <mctp/util/alloc.h>
#include <stdlib.h>
#include <string.h>

mctp_packet_t *mctp_pkt_create(
    const mctp_transport_header_t *header,
    const uint8_t payload_data[],
    const size_t payload_len
) {
    mctp_packet_t *packet = _alloc(mctp_packet_t);

    memcpy(&packet->header, header, sizeof(mctp_transport_header_t));
    memcpy(packet->payload, payload_data, payload_len);
    packet->len = payload_len + sizeof(mctp_transport_header_t);

    return packet;
}

mctp_packet_t *mctp_pkt_clone(
    const mctp_packet_t *packet
) {
    mctp_packet_t *duplicate = _alloc(mctp_packet_t);

    memcpy(duplicate, packet, sizeof(mctp_packet_t));

    return duplicate;
}

void mctp_pkt_destroy(
    mctp_packet_t *packet
) {
    free(packet);
}

bool mctp_pkt_message_match(
    const mctp_packet_t *packet,
    const mctp_msg_ctx_t *message_ctx
) {
    return
        packet->header.source == message_ctx->eid &&
        packet->header.tag == message_ctx->tag &&
        packet->header.tag_owner == message_ctx->tag_owner;
}
