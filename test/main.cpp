#include <catch2/catch_test_macros.hpp>
#include <stdio.h>
#include "dump.hpp"

extern "C" {
    #include <mctp/control/message.h>
    #include <mctp/binding/serial.h>
    #include <mctp/core/bus.h>
}


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
    mctp_serial_byte_rx(test_env.binding, byte);
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
        .message_tag = mctp_get_message_tag(),
        .tag_owner = true
    };
    const uint8_t message_data[] = {
        0x7E, 0x7D, 0x5D, 0x5E
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


TEST_CASE( "MCTP tx control request", "[tx]" ) {
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
