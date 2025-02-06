#include <catch2/catch_test_macros.hpp>
#include <stdio.h>
#include "dump.hpp"

extern "C" {
    #include <mctp/control/message.h>
    #include <mctp/binding/serial.h>
    #include <mctp/core/bus.h>
}


void map_serial_buffer(
    mctp_serial_buffer_map_t *buffer_map,
    const uint8_t buffer_data[]
) {
    buffer_map->header = (mctp_serial_header_t *)buffer_data;
    buffer_map->packet = (uint8_t *)(buffer_map->header + 1);
    buffer_map->trailer = (mctp_serial_trailer_t *)(buffer_map->packet + buffer_map->header->byte_count);
}

void serial_buffer_tx(
    const uint8_t buffer_data[],
    const size_t buffer_len
) {
    mctp_serial_buffer_map_t buffer_map = {};
    map_serial_buffer(&buffer_map, buffer_data);

    serial_header_dump(buffer_map.header);
    pkt_header_dump((mctp_transport_header_t *)buffer_map.packet);
    serial_trailer_dump(buffer_map.trailer);
}


TEST_CASE( "MCTP tx message", "[tx]" ) {
    // Endpoint setup
    mctp_eid_t eid_source = 0xA;
    mctp_eid_t eid_dest = 0xB;
    mctp_pktq_t tx_queue = {};

    mctp_bus_t *bus = mctp_bus_create();
    REQUIRE( bus != NULL );

    mctp_serial_t *serial = mctp_serial_create();
    REQUIRE( serial != NULL );

    mctp_binding_t *binding = mctp_serial_get_binding(serial);
    REQUIRE( binding != NULL );

    mctp_bus_set_eid(bus, eid_source);
    mctp_bus_transport_bind(bus, binding);
    mctp_serial_set_buffer_tx(serial, serial_buffer_tx);

    // Prepare message
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

    // Fill tx queue
    mctp_message_disassemble(
        &tx_queue,
        bus,
        &message_ctx,
        message_data,
        sizeof(message_data)
    );

    // Drain tx queue
    mctp_pktq_drain(&tx_queue, bus);

    // Clean up
    mctp_bus_destroy(bus);
    mctp_serial_destroy(serial);
}


TEST_CASE( "MCTP tx control request", "[tx]" ) {
    // Endpoint setup
    mctp_eid_t eid_source = 0xA;
    mctp_eid_t eid_dest = 0xB;
    mctp_pktq_t tx_queue = {};

    mctp_bus_t *bus = mctp_bus_create();
    REQUIRE( bus != NULL );

    mctp_serial_t *serial = mctp_serial_create();
    REQUIRE( serial != NULL );

    mctp_binding_t *binding = mctp_serial_get_binding(serial);
    REQUIRE( binding != NULL );

    mctp_bus_set_eid(bus, eid_source);
    mctp_bus_transport_bind(bus, binding);
    mctp_serial_set_buffer_tx(serial, serial_buffer_tx);

    // Prepare request
    mctp_req_get_mctp_ver_t payload = {
        .msg_type = MCTP_MSG_TYPE_PLDM
    };
    mctp_ctrl_cmd_t command = MCTP_CTRL_CMD_GET_VERSION_SUPPORT;

    // Fill tx queue
    mctp_ctrl_request_prepare(
        &tx_queue,
        bus,
        eid_dest,
        command,
        false,
        (uint8_t *)&payload,
        sizeof(payload)
    );

    // Drain tx queue
    mctp_pktq_drain(&tx_queue, bus);

    // Clean up
    mctp_bus_destroy(bus);
    mctp_serial_destroy(serial);
}
