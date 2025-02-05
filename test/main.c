#include <mctp/control/message.h>
#include <stdio.h>


void mctp_pkt_header_dump(
    const mctp_transport_header_t *header
);

void send_ctrl_request();
void send_large_message();


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
    printf("\n");
}

void send_ctrl_request()
{
    printf("----------Test send_ctrl_request started----------\n");

    mctp_eid_t eid_source = 0xA;
    mctp_eid_t eid_dest = 0xB;
    mctp_pktq_t tx_queue = {};

    mctp_bus_t *bus = mctp_bus_create();
    mctp_bus_set_eid(bus, eid_source);


    mctp_req_get_mctp_ver_t payload = {
        .msg_type = MCTP_MSG_TYPE_PLDM
    };

    mctp_ctrl_request_prepare(
        &tx_queue,
        bus,
        eid_dest,
        MCTP_CTRL_CMD_GET_VERSION_SUPPORT,
        false,
        (uint8_t*)&payload,
        sizeof(mctp_req_get_mctp_ver_t)
    );

    mctp_pktq_drain(&tx_queue, bus);
    mctp_bus_destroy(bus);

    printf("----------Test finished----------\n\n");
}

void send_large_message() {
    printf("----------Test send_ctrl_request started----------\n");

    mctp_eid_t eid_source = 0xA;
    mctp_eid_t eid_dest = 0xB;
    mctp_pktq_t tx_queue = {};
    
    mctp_bus_t *bus = mctp_bus_create();
    mctp_bus_set_eid(bus, eid_source);


    const mctp_msg_ctx_t message_ctx = {
        .eid = eid_dest,
        .message_tag = mctp_get_message_tag(),
        .tag_owner = true
    };

    const uint8_t message_data[] =
        "This is a very long string that exceeds 256 bytes. "
        "You can split it into multiple quoted segments, and the compiler will "
        "automatically concatenate them into a single string. "
        "This avoids line length limits while keeping the code readable.";

    mctp_message_disassemble(
        &tx_queue,
        bus,
        &message_ctx,
        message_data,
        sizeof(message_data)
    );


    mctp_pktq_drain(&tx_queue, bus);
    mctp_bus_destroy(bus);

    printf("----------Test finished----------\n\n");
}

int main() {
    send_ctrl_request();
    send_large_message();
}
