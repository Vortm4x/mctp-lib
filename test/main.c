#include <mctp/control/message.h>


int main() {
    mctp_eid_t eid_source = 0xA;
    mctp_eid_t eid_dest = 0xB;

    mctp_bus_t *bus = mctp_bus_create();
    mctp_bus_set_eid(bus, eid_source);

    mctp_req_get_mctp_ver_t payload = {
        .msg_type = MCTP_MSG_TYPE_PLDM
    };

    mctp_pktq_t tx_queue = {};

    mctp_ctrl_request_tx(
        &tx_queue,
        bus,
        eid_dest,
        MCTP_CTRL_CMD_GET_VERSION_SUPPORT,
        false,
        (uint8_t*)&payload,
        sizeof(mctp_req_get_mctp_ver_t)
    );

    while (!mctp_pktq_empty(&tx_queue))
    {
        mctp_pktq_node_t *node = mctp_pktq_pop(&tx_queue);

        mctp_packet_tx(bus, node);

        mctp_pktq_node_destroy(node);
    }

    mctp_bus_destroy(bus);
}
