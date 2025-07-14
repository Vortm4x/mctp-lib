#include <mctp/control/message.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void mctp_ctrl_request_prepare(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const mctp_eid_t dest,
    const mctp_ctrl_cmd_t command,
    const bool datagram,
    const uint8_t payload_data[],
    const size_t payload_len
) {
    static uint8_t curr_inst = 0;

    const mctp_ctrl_header_t header = {
        .base = {
            .integrity_check = false,
            .type = MCTP_MSG_TYPE_CONTROL,
        },
        .instance = curr_inst++,
        .datagram = datagram,
        .request = true,
        .command = command
    };

    const mctp_msg_ctx_t message_ctx = {
        .eid = dest,
        .tag = mctp_get_message_tag(),
        .tag_owner = true
    };

    mctp_ctrl_message_prepare(
        tx_queue,
        bus,
        &message_ctx,
        &header,
        payload_data,
        payload_len
    );
}

void mctp_ctrl_message_prepare(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx,
    const mctp_ctrl_header_t *header,
    const uint8_t payload_data[],
    const size_t payload_len
) {
    const size_t message_len = sizeof(mctp_ctrl_header_t) + payload_len;
    uint8_t message_data[message_len];

    memcpy(message_data, header, sizeof(mctp_ctrl_header_t));
    memcpy(message_data + sizeof(mctp_ctrl_header_t), payload_data, payload_len);

    mctp_message_disassemble(
        tx_queue,
        bus,
        message_ctx,
        message_data,
        message_len
    );
}

void mctp_ctrl_header_dump(
    const mctp_ctrl_header_t *header
) {
    printf("MCTP Control header\n");
    printf("instance:   0x%02X\n",  header->instance);
    printf("datagram:   %s\n",      header->datagram ? "YES" : "NO");
    printf("type:       %s\n",      header->request ? "request" : "response");
    printf("command:    0x%02X\n",  header->command);
}
