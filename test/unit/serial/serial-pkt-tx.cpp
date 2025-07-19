#include <catch2/catch_test_macros.hpp>
#include <unit/serial/test-common.h>
#include <stdio.h>
#include <stddef.h>

#include <mctp/binding/serial.h>
#include <mctp/core/bus.h>
#include <mctp/core/packet.h>
#include <mctp/core/mctp.h>
#include <mctp/util/crc16.h>


constexpr mctp_eid_t TEST_EID_SOURCE        = 0xA;
constexpr uint8_t TEST_RANDOM_BYTE          = 0x89;
constexpr uint8_t TEST_PKT_PAYLOAD_SIZE     = 3;
constexpr uint8_t TEST_PKT_HEADERS_SIZE     = MCTP_PKT_MIN_SIZE;
constexpr uint8_t TEST_BYTE_COUNT           = TEST_PKT_HEADERS_SIZE + TEST_PKT_PAYLOAD_SIZE;
constexpr uint8_t TEST_ESCAPED_PAYLOAD_SIZE = TEST_PKT_PAYLOAD_SIZE + 2;


constexpr test_io_struct_wrapper_t<
    test_crc_struct_t<TEST_PKT_PAYLOAD_SIZE>
>
TEST_CRC_STRUCT = {
    .fields = {
        .packet = {
            .payload = {
                MCTP_SERIAL_ESCAPE_FLAG,
                MCTP_SERIAL_FRAME_FLAG,
                TEST_RANDOM_BYTE
            }
        }
    }
};

const uint16_t TEST_CRC_VAL = crc16_calc_block(
    MCTP_CRC16_INIT,
    TEST_CRC_STRUCT.data,
    sizeof(TEST_CRC_STRUCT.data)
);

const test_io_struct_wrapper_t<
    test_serial_frame_t<TEST_ESCAPED_PAYLOAD_SIZE>
>
TEST_SERIAL_FRAME = {
    .fields = {
        .serial_header = {
            .framing_flag = MCTP_SERIAL_FRAME_FLAG,
            .revision = MCTP_SERIAL_REVISION,
            .byte_count = TEST_BYTE_COUNT
        },
        .packet = {
            .payload = {
                // Escaped "escape flag" (0x7D -> 0x7D 0x5D)
                MCTP_SERIAL_ESCAPE_FLAG, 
                MCTP_SERIAL_ESCAPE_BYTE(MCTP_SERIAL_ESCAPE_FLAG),
                // Escaped "frame flag" (0x7E -> 0x7D 0x5E)
                MCTP_SERIAL_ESCAPE_FLAG, 
                MCTP_SERIAL_ESCAPE_BYTE(MCTP_SERIAL_FRAME_FLAG),
                // Regular data
                TEST_RANDOM_BYTE
            }
        },
        .serial_trailer = {
            .fcs_high = MCTP_CRC16_GET_HIGH(TEST_CRC_VAL),
            .fcs_low = MCTP_CRC16_GET_LOW(TEST_CRC_VAL),
            .framing_flag = MCTP_SERIAL_FRAME_FLAG,
        }
    }
};

constexpr mctp_packet_t TEST_PKT = {
    .io = {
        .payload = {
            MCTP_SERIAL_ESCAPE_FLAG,
            MCTP_SERIAL_FRAME_FLAG,
            TEST_RANDOM_BYTE
        }
    },
    .len = TEST_BYTE_COUNT
};

struct {
    FILE* stream;
    size_t size;
    union {
        uint8_t* buffer;
        char* c_buffer;
    };
}
TEST_RX_CTX = {};


static void test_serial_byte_tx(
    const uint8_t byte
) {
    fputc(byte, TEST_RX_CTX.stream);
}


TEST_CASE("serial-pkt-tx") {
    mctp_bus_t *bus = NULL;
    mctp_serial_t *serial = NULL;
    mctp_binding_t *binding = NULL;

    // Bus setup
    bus = mctp_bus_create();
    mctp_bus_set_eid(bus, TEST_EID_SOURCE);

    serial = mctp_serial_create();
    binding = mctp_serial_get_binding(serial);
    mctp_bus_transport_bind(bus, binding);
    mctp_serial_set_byte_tx(serial, test_serial_byte_tx);

    // Open data stream
    TEST_RX_CTX.stream = open_memstream(
        &TEST_RX_CTX.c_buffer,
        &TEST_RX_CTX.size
    );

    // Send packet
    mctp_packet_tx(bus, &TEST_PKT);

    // Close data stream
    fclose(TEST_RX_CTX.stream);

    REQUIRE(sizeof(TEST_SERIAL_FRAME.data) == TEST_RX_CTX.size);
    REQUIRE(memcmp(TEST_SERIAL_FRAME.data, TEST_RX_CTX.buffer, TEST_RX_CTX.size) == 0);

    // Clean up
    mctp_serial_destroy(serial);
    mctp_bus_destroy(bus);
}
