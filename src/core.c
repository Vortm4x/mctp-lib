#include <mctp/core/mctp.h>
#include <mctp/core/binding.h>
#include <string.h>


uint8_t mctp_get_message_tag()
{
    static uint8_t msg_tag = 0;

    return msg_tag++;
}

void mctp_message_disassemble(
    mctp_pktq_t *tx_queue,
    const mctp_eid_t source,
    const mctp_msg_ctx_t *message_ctx,
    const uint8_t message_data[],
    const size_t message_len
) {
    mctp_transport_header_t header = {
        .version = MCTP_PKT_HDR_VER,
        .dest = message_ctx->eid,
        .source = source,
        .tag = message_ctx->message_tag,
        .tag_owner = message_ctx->tag_owner,
    };

    const size_t packet_count = (message_len / MCTP_BASE_MTU) + 1;

    for(size_t i = 0; i < packet_count; ++i)
    {
        header.som = (i == 0);
        header.eom = (i + 1 == packet_count);

        const uint8_t *payload_data = &message_data[MCTP_BASE_MTU * i];
        const size_t payload_len = header.eom 
            ? message_len - MCTP_BASE_MTU * i
            : MCTP_BASE_MTU;

        mctp_pktq_node_t *node = mctp_pktq_node_create(
            &header,
            payload_data,
            payload_len
        );

        mctp_pktq_push(tx_queue, node);

        header.pkt_seq++;
    }
}

void mctp_message_tx(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx,
    const uint8_t message_data[],
    const size_t message_len
) {
    mctp_message_disassemble(
        tx_queue,
        bus->eid,
        message_ctx,
        message_data,
        message_len
    );
}

#include <stdio.h>
void mctp_packet_tx(
    const mctp_bus_t *bus,
    const mctp_pktq_node_t *node
) {
    const mctp_transport_header_t *header = &(node->packet.header);

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
