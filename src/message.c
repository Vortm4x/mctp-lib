#include <mctp/core/p_message.h>
#include <mctp/core/p_packet.h>
#include <mctp/core/p_bus.h>
#include <mctp/core/packet_queue.h>
#include <string.h>


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

    mctp_pkt_hdr_t header = {
        .version    = MCTP_PKT_HDR_VER,
        .dest       = dest,
        .source     = source,
        .tag        = (uint8_t)(msg_tag & 0x7),
        .tag_owner  = tag_owner,
        .seq_num    = 0
    };

    const size_t pkt_count =
        (msg_len / MCTP_BASE_MTU) +
        (msg_len % MCTP_BASE_MTU ? 1 : 0);

    for (size_t i = 0; i < pkt_count; ++i)
    {
        header.som = (i == 0);
        header.eom = ((i + 1) == pkt_count);

        const uint8_t *payload_data = &msg_data[MCTP_BASE_MTU * i];

        const mctp_pkt_size_t payload_len = header.eom
            ? (mctp_pkt_size_t)(msg_len - (MCTP_BASE_MTU * i))
            : MCTP_BASE_MTU;

        mctp_pkt_t *packet = mctp_pkt_create();
        memcpy(&packet->io.header, &header, MCTP_PKT_HDR_SIZE);
        memcpy(&packet->io.body, payload_data, payload_len);
        packet->len = MCTP_PKT_HDR_SIZE + payload_len;

        mctp_pktq_enqueue(tx_queue, packet);

        header.seq_num++;
    }
}
