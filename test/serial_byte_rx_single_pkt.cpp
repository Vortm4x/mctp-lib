#include <catch2/catch_test_macros.hpp>
#include <string.h>

#include <mctp/binding/serial.h>
#include <mctp/core/bus.h>
#include <mctp/util/crc16.h>

void serial_buff_rx(
    const mctp_binding_t *binding,
	const uint8_t buffer_data[],
    const size_t buffer_len
) {
    for(size_t i = 0; i < buffer_len; ++i)
    {
        mctp_serial_byte_rx(binding, buffer_data[i]);
    }
}

TEST_CASE( "serial-byte-rx-pkt" ) {
    // Setup transport and endpoint
    const mctp_eid_t EID_SOURCE = 0xA;
    const uint8_t PKT_DATA[] = {
        0x0, 0x1, 0x2, 0x3,
        0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF,
        0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF,
        0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF,
        0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF,
    };
    const size_t PKT_SIZE = sizeof(PKT_DATA);


    mctp_bus_t *bus = NULL;
    mctp_serial_t *serial = NULL;
    mctp_binding_t *binding = NULL;

    bus = mctp_bus_create();
    REQUIRE( bus != NULL );

    serial = mctp_serial_create();
    REQUIRE( serial != NULL );

    binding = mctp_serial_get_binding(serial);
    REQUIRE( binding != NULL );

    mctp_bus_set_eid(bus, EID_SOURCE);
    mctp_bus_transport_bind(bus, binding);

    // Init serial frame
    const mctp_serial_header_t header = {
        .framing_flag = MCTP_SERIAL_FRAME_FLAG,
        .revision = MCTP_SERIAL_REVISION,
        .byte_count = PKT_SIZE
    };

    static uint16_t crc = 0;

    crc = MCTP_CRC16_INIT;
    crc = crc16_calc_byte(crc, MCTP_SERIAL_REVISION);
    crc = crc16_calc_byte(crc, PKT_SIZE);
    crc = crc16_calc_block(crc, PKT_DATA, PKT_SIZE);

    const mctp_serial_trailer_t trailer = {
        .fcs_high = MCTP_CRC16_GET_HIGH(crc),
        .fcs_low = MCTP_CRC16_GET_LOW(crc),
        .framing_flag = MCTP_SERIAL_FRAME_FLAG,
    };

    // Rx frame
    serial_buff_rx(binding, (uint8_t *)&header, sizeof(header));
    serial_buff_rx(binding, PKT_DATA, PKT_SIZE);
    serial_buff_rx(binding, (uint8_t *)&trailer, sizeof(trailer));

    REQUIRE_FALSE( mctp_pktq_empty(&bus->rx.packet_queue) );

    mctp_packet_t* rx_packet = mctp_pktq_dequeue(&bus->rx.packet_queue);

    REQUIRE( rx_packet->len == PKT_SIZE );

    REQUIRE( memcmp(PKT_DATA, rx_packet->data, PKT_SIZE) == 0 );
}