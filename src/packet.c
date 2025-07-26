#include <mctp/core/packet.h>
#include <mctp/util/alloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

bool mctp_pkt_message_match(
    const mctp_packet_t *packet,
    const mctp_msg_ctx_t *message_ctx
) {
    return
        packet->io.header.source == message_ctx->eid &&
        packet->io.header.tag == message_ctx->tag &&
        packet->io.header.tag_owner == message_ctx->tag_owner;
}

void mctp_pkt_header_dump(
    const mctp_transport_header_t *header
) {
    printf("MCTP Transport header\n");
    printf("version:    0x%X\n",    header->version);
    printf("dest:       0x%02X\n",  header->dest);
    printf("source:     0x%02X\n",  header->source);
    printf("tag:        0x%d\n",    header->tag);
    printf("tag_owner:  0x%s\n",    header->tag_owner ? "YES" : "NO");
    printf("pkt_seq:    0x%d\n",    header->pkt_seq);
    printf("som:        0x%s\n",    header->som ? "YES" : "NO");
    printf("eom:        0x%s\n",    header->eom ? "YES" : "NO");
}
