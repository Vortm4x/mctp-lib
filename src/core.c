#include <mctp/core/mctp.h>
#include <mctp/core/binding.h>
#include <string.h>


uint8_t mctp_get_message_tag()
{
    static uint8_t msg_tag = 0;

    return msg_tag++;
}

mctp_pktq_node_t* mctp_message_disassemble(
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
    mctp_pktq_t packet_queue = {};

    for(size_t i = 0; i < packet_count; ++i)
    {
        header.som = (i == 0);
        header.eom = (i + 1 == packet_count);

        const uint8_t* payload_data = &message_data[MCTP_BASE_MTU * i];
        const size_t payload_len = header.eom 
            ? (MCTP_BASE_MTU * i) - message_len
            : MCTP_BASE_MTU;

        mctp_pktq_node_t *node = mctp_pktq_node_create(
            &header,
            payload_data,
            payload_len
        );

        mctp_pktq_push(&packet_queue, node);

        header.pkt_seq++;
    }

    return packet_queue.head;
}

void mctp_message_tx(
    const mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx,
    const uint8_t message_data[],
    const size_t message_len
) {
    mctp_pktq_node_t* pktq_head = mctp_message_disassemble(
        bus->eid,
        message_ctx,
        message_data,
        message_len
    );

    mctp_pktq_push(&bus->binding->tx_queue, pktq_head);

    // TO DO: mctp_packet_tx
}
