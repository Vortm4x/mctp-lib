#include <catch2/catch_test_macros.hpp>
#include <stdio.h>
#include "dump.hpp"

#include <mctp/control/message.h>
#include <mctp/binding/serial.h>
#include <mctp/core/bus.h>


struct
{
    mctp_bus_t *bus = NULL;
    mctp_serial_t *serial = NULL;
    mctp_binding_t *binding = NULL;
} 
test_env;


void serial_byte_tx(
    const uint8_t byte
) {
    static mctp_pktq_t msg_pktq = {};

    mctp_serial_byte_rx(test_env.binding, byte);

    if(!mctp_msgq_empty(&test_env.bus->rx.message_queue))
    {
        const mctp_msg_ctx_t msg_ctx = mctp_msgq_dequeue(&test_env.bus->rx.message_queue);

        mctp_pktq_rx(test_env.bus, &msg_pktq, &msg_ctx);

        while (!mctp_pktq_empty(&msg_pktq))
        {
            mctp_packet_t *packet = mctp_pktq_dequeue(&msg_pktq);
            pkt_header_dump(&packet->header);

            mctp_pkt_destroy(packet);
        }
    }
}


TEST_CASE( "MCTP tx message", "[tx]" ) {
    // Endpoint setup
    mctp_eid_t eid_source = 0xA;
    mctp_eid_t eid_dest = 0xB;
    mctp_pktq_t tx_queue = {};

    test_env.bus = mctp_bus_create();
    REQUIRE( test_env.bus != NULL );

    test_env.serial = mctp_serial_create();
    REQUIRE( test_env.serial != NULL );

    test_env.binding = mctp_serial_get_binding(test_env.serial);
    REQUIRE( test_env.binding != NULL );

    mctp_bus_set_eid(test_env.bus, eid_source);
    mctp_bus_transport_bind(test_env.bus, test_env.binding);
    mctp_serial_set_byte_tx(test_env.serial, serial_byte_tx);

    // Prepare message
    const mctp_msg_ctx_t message_ctx = {
        .eid = eid_dest,
        .tag = mctp_get_message_tag(),
        .tag_owner = true
    };
    const uint8_t message_data[] = {
        0x7E, 0x31, 0x42, 0x53, 0x7D, 0xA1, 0x5D, 0xC7, 0x5E, 0x12, 0x33, 0x99, 
        0x7E, 0x45, 0x6A, 0x7D, 0x6F, 0x5D, 0x2B, 0x5E, 0x99, 0x56, 0x7E, 0x73, 
        0x0A, 0x7D, 0x3D, 0x5D, 0x22, 0x5E, 0xBC, 0x0F, 0x7E, 0xA9, 0x7D, 0xF3, 
        0x5D, 0xD2, 0x5E, 0x56, 0x7E, 0x31, 0x65, 0x7D, 0x28, 0x5D, 0x90, 0x5E, 
        0x5B, 0x7E, 0x11, 0x7D, 0xF4, 0x5D, 0x34, 0x5E, 0xC1, 0x7E, 0x9E, 0x7D, 
        0x76, 0x5D, 0x1E, 0x5E, 0x28, 0x7E, 0x91, 0x7D, 0xC8, 0x5D, 0xD5, 0x5E, 
        0x02, 0x7E, 0x49, 0x7D, 0x7F, 0x5D, 0x60, 0x5E, 0xE4, 0x7E, 0x56, 0x7D, 
        0x23, 0x5D, 0x47, 0x5E, 0xDF, 0x7E, 0xB2, 0x7D, 0x13, 0x5D, 0x88, 0x5E, 
        0x6F, 0x7E, 0x47, 0x7D, 0x8D, 0x5D, 0x9A, 0x5E, 0x76, 0x7E, 0x68, 0x7D, 
        0x55, 0x5D, 0x92, 0x5E, 0x4C, 0x7E, 0x6B, 0x34, 0x5E, 0xC1, 0x7E, 0x9E, 
        0x76, 0x5D, 0x1E, 0x5E, 0x28, 0x7E, 0x91, 0x7D, 0xC8, 0x5D, 0xD5, 0x5E, 
        0x02, 0x7E, 0x49, 0x7D, 0x7F, 0x5D, 0x60, 0x5E, 0xE4, 0x7E, 0x56, 0x7D, 
        0x23, 0x5D, 0x47, 0x5E, 0xDF, 0x7E, 0xB2, 0x7D, 0x13, 0x5D, 0x88, 0x5E, 
        0x6F, 0x7E, 0x47, 0x7D, 0x8D, 0x5D, 0x9A, 0x5E, 0x76, 0x7E, 0x68, 0x7D, 
        0x55, 0x5D, 0x92, 0x5E, 0x4C, 0x7E, 0x6B
    };

    // Fill tx queue
    mctp_message_disassemble(
        &tx_queue,
        test_env.bus,
        &message_ctx,
        message_data,
        sizeof(message_data)
    );

    // Drain tx queue
    mctp_pktq_drain(&tx_queue, test_env.bus);

    // Clean up
    mctp_bus_destroy(test_env.bus);
    mctp_serial_destroy(test_env.serial);

    memset(&test_env, 0, sizeof(test_env));
}


TEST_CASE( "MCTP Control message", "[tx]" ) {
    // Endpoint setup
    mctp_eid_t eid_source = 0xA;
    mctp_eid_t eid_dest = 0xB;
    mctp_pktq_t tx_queue = {};

    test_env.bus = mctp_bus_create();
    REQUIRE( test_env.bus != NULL );

    test_env.serial = mctp_serial_create();
    REQUIRE( test_env.serial != NULL );

    test_env.binding = mctp_serial_get_binding(test_env.serial);
    REQUIRE( test_env.binding != NULL );

    mctp_bus_set_eid(test_env.bus, eid_source);
    mctp_bus_transport_bind(test_env.bus, test_env.binding);
    mctp_serial_set_byte_tx(test_env.serial, serial_byte_tx);

    // Prepare request
    mctp_req_get_mctp_ver_t payload = {
        .msg_type = MCTP_MSG_TYPE_PLDM
    };
    mctp_ctrl_cmd_t command = MCTP_CTRL_CMD_GET_VERSION_SUPPORT;

    // Fill tx queue
    mctp_ctrl_request_prepare(
        &tx_queue,
        test_env.bus,
        eid_dest,
        command,
        false,
        (uint8_t *)&payload,
        sizeof(payload)
    );

    // Drain tx queue
    mctp_pktq_drain(&tx_queue, test_env.bus);

    // Clean up
    mctp_bus_destroy(test_env.bus);
    mctp_serial_destroy(test_env.serial);

    memset(&test_env, 0, sizeof(test_env));
}


TEST_CASE( "XXX "){}