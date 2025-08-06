#include <mctp/core/p_message.h>
#include <mctp/core/p_packet.h>
#include <mctp/core/p_bus.h>
#include <mctp/core/packet_queue.h>
#include <string.h>
#include <stdlib.h>


void mctp_fill_send_queue(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const uint8_t *msg_data,
    const size_t msg_len,
    const mctp_eid_t dest
) {
    if (bus == NULL) return;

    static uint8_t msg_tag = 0;

    mctp_message_disassemble(
        tx_queue,
        msg_data,
        msg_len,
        bus->eid,
        dest,
        msg_tag++,
        MCTP_TAG_OWNER_SEND
    );
}

void mctp_fill_reply_queue(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const uint8_t *msg_data,
    const size_t msg_len,
    const mctp_eid_t dest,
    const uint8_t msg_tag
) {
    if (bus == NULL) return;

    mctp_message_disassemble(
        tx_queue,
        msg_data,
        msg_len,
        bus->eid,
        dest,
        msg_tag,
        MCTP_TAG_OWNER_REPLY
    );
}

void mctp_message_disassemble(
    mctp_pktq_t *tx_queue,
    const uint8_t *msg_data,
    const size_t msg_len,
    const mctp_eid_t source,
    const mctp_eid_t dest,
    const uint8_t msg_tag,
    const bool tag_owner
) {
    if (tx_queue == NULL) return;

    mctp_hdr_t header = {
            .version    = MCTP_PKT_HDR_VER,
            .dest       = dest,
            .source     = source,
            .flags      = MCTP_HDR_FLAG_SET_TAG_OWNER(0, tag_owner)
                        | MCTP_HDR_FLAG_SET_MSG_TAG(0, msg_tag)
    };

    const size_t pkt_count =
        (msg_len / MCTP_BASE_MTU) +
        (msg_len % MCTP_BASE_MTU ? 1 : 0);

    for (size_t i = 0; i < pkt_count; ++i)
    {
        const bool is_som = (i == 0);
        const bool is_eom = ((i + 1) == pkt_count);

        header.flags = MCTP_HDR_FLAG_SET_SOM(header.flags, is_som);
        header.flags = MCTP_HDR_FLAG_SET_EOM(header.flags, is_eom);

        const uint8_t *payload_data = &msg_data[MCTP_BASE_MTU * i];

        const mctp_pkt_size_t payload_len = is_eom
            ? (mctp_pkt_size_t)(msg_len - (MCTP_BASE_MTU * i))
            : MCTP_BASE_MTU;

        mctp_pkt_t *packet = mctp_pkt_create();
        memcpy(&packet->io.header, &header, MCTP_PKT_HDR_SIZE);
        memcpy(packet->io.body, payload_data, payload_len);
        packet->len = MCTP_PKT_HDR_SIZE + payload_len;

        mctp_pktq_enqueue(tx_queue, packet);

        header.flags = MCTP_HDR_FLAG_SET_SEQ_NUM(
            header.flags,
            MCTP_HDR_FLAG_GET_SEQ_NUM(header.flags) + 1
        );
    }
}

void mctp_message_assemble(
    const mctp_pktq_t *rx_queue,
    uint8_t **message_data,
    size_t *message_len
) {
    mctp_pktq_node_t *node = NULL;
    uint8_t *curr_data = NULL;

    *message_len = 0;
    node = mctp_pktq_front(rx_queue);

    while (node != NULL)
    {
        const mctp_pkt_t *packet = mctp_pktq_node_data(node);

        *message_len += packet->len;

        node = mctp_pktq_node_next(node);
    }

    *message_data = malloc(*message_len);
    curr_data = *message_data;
    node = mctp_pktq_front(rx_queue);

    while (node != NULL)
    {
        const mctp_pkt_t *packet = mctp_pktq_node_data(node);
        const mctp_pkt_size_t body_len = packet->len - MCTP_PKT_HDR_SIZE;

        memcpy(curr_data, packet->io.body, body_len);
        curr_data += body_len;

        node = mctp_pktq_node_next(node);
    }
}
