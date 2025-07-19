#include <catch2/catch_test_macros.hpp>
#include <string.h>

#include <mctp/binding/serial.h>
#include <mctp/core/bus.h>
#include <mctp/core/packet.h>
#include <mctp/util/crc16.h>


#define TEST_PACKET_DATA                                \
    {                                                   \
        0x00, 0x01, 0x02, 0x03,                         \
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, \
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, \
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, \
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, \
        0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, \
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, \
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F  \
    }

#define TEST_IO_STRUCT_LAYOUT(...)      \
union                                   \
{                                       \
    struct __attribute__ ((__packed__)) \
    {                                   \
        __VA_ARGS__                     \
    }                                   \
    FIELDS;                             \
                                        \
    uint8_t DATA[sizeof(FIELDS)];       \
}


constexpr mctp_eid_t TEST_EID_SOURCE    = 0xA;
constexpr mctp_eid_t TEST_EID_DEST      = 0xB;
constexpr size_t TEST_PKT_SIZE          = MCTP_PKT_MAX_SIZE;
constexpr size_t TEST_FRAME_SIZE        = TEST_PKT_SIZE;

constexpr TEST_IO_STRUCT_LAYOUT(
    uint8_t revision;
    uint8_t byte_count;
    mctp_io_packet_t packet;
)
TEST_CRC_STRUCT = {
    .FIELDS = {
        .revision = MCTP_SERIAL_REVISION,
        .byte_count = TEST_PKT_SIZE,
        .packet = {
            .data = TEST_PACKET_DATA
        }
    }
};

const uint16_t TEST_CRC_VAL = crc16_calc_block(
    MCTP_CRC16_INIT,
    TEST_CRC_STRUCT.DATA,
    sizeof(TEST_CRC_STRUCT.DATA)
);

const TEST_IO_STRUCT_LAYOUT(
    mctp_serial_header_t header;
    uint8_t packet[TEST_FRAME_SIZE];
    mctp_serial_trailer_t trailer;
)
TEST_SERIAL_FRAME = {
    .FIELDS = {
        .header = {
            .framing_flag = MCTP_SERIAL_FRAME_FLAG,
            .revision = MCTP_SERIAL_REVISION,
            .byte_count = TEST_PKT_SIZE
        },
        .packet = TEST_PACKET_DATA,
        .trailer = {
            .fcs_high = MCTP_CRC16_GET_HIGH(TEST_CRC_VAL),
            .fcs_low = MCTP_CRC16_GET_LOW(TEST_CRC_VAL),
            .framing_flag = MCTP_SERIAL_FRAME_FLAG,
        }
    }
};


static void serial_buff_rx(
    const mctp_binding_t *binding,
	const uint8_t buffer_data[],
    const size_t buffer_len
) {
    for(size_t i = 0; i < buffer_len; ++i)
    {
        mctp_serial_byte_rx(binding, buffer_data[i]);
    }
}


TEST_CASE("serial-frame-rx") {
    mctp_bus_t *bus = NULL;
    mctp_serial_t *serial = NULL;
    mctp_binding_t *binding = NULL;

    // Bus setup
    bus = mctp_bus_create();
    mctp_bus_set_eid(bus, TEST_EID_SOURCE);

    serial = mctp_serial_create();
    binding = mctp_serial_get_binding(serial);
    mctp_bus_transport_bind(bus, binding);

    // Read frame
    serial_buff_rx(binding, TEST_SERIAL_FRAME.DATA, sizeof(TEST_SERIAL_FRAME.DATA));
    REQUIRE_FALSE(mctp_pktq_empty(&bus->rx.packet_queue));

    // Deque packet
    mctp_packet_t* rx_packet = mctp_pktq_dequeue(&bus->rx.packet_queue);

    REQUIRE(rx_packet->len == TEST_PKT_SIZE);
    REQUIRE(memcmp(TEST_CRC_STRUCT.FIELDS.packet.data, rx_packet->io.data, TEST_PKT_SIZE) == 0);

    // Clean up
    mctp_pkt_destroy(rx_packet);
    mctp_serial_destroy(serial);
    mctp_bus_destroy(bus);
}
