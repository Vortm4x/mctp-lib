#include <catch2/catch_test_macros.hpp>

#include <mctp/binding/serial.h>
#include <mctp/core/bus.h>
#include <mctp/core/packet.h>
#include <mctp/util/crc16.h>


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


constexpr mctp_eid_t TEST_EID_SOURCE        = 0xA;
constexpr uint8_t TEST_RANDOM_BYTE          = 0x89;
constexpr uint8_t TEST_PKT_PAYLOAD_SIZE     = 3;
constexpr uint8_t TEST_PKT_HEADERS_SIZE     = MCTP_PKT_MIN_SIZE;
constexpr uint8_t TEST_BYTE_COUNT           = TEST_PKT_HEADERS_SIZE + TEST_PKT_PAYLOAD_SIZE;
constexpr uint8_t TEST_ESCAPED_PAYLOAD_SIZE = TEST_PKT_PAYLOAD_SIZE + 2;


constexpr TEST_IO_STRUCT_LAYOUT(
    mctp_transport_header_t transport_header;
    uint8_t payload[TEST_ESCAPED_PAYLOAD_SIZE];
)
TEST_FRAME_PACKET = {
    .FIELDS = {
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
    }
};

constexpr TEST_IO_STRUCT_LAYOUT(
    uint8_t revision;
    uint8_t byte_count;

    union {
        struct {
            mctp_transport_header_t transport_header;
            uint8_t payload[TEST_PKT_PAYLOAD_SIZE];
        };
        uint8_t data[TEST_BYTE_COUNT];
    } packet;
)
TEST_CRC_STRUCT = {
    .FIELDS = {
        .revision = MCTP_SERIAL_REVISION,
        .byte_count = TEST_BYTE_COUNT,
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
    TEST_CRC_STRUCT.DATA,
    sizeof(TEST_CRC_STRUCT.DATA)
);


TEST_CASE("serial-rx-fsm") {
    mctp_bus_t *bus = NULL;
    mctp_serial_t *serial = NULL;
    mctp_binding_t *binding = NULL;

    // Bus setup
    bus = mctp_bus_create();
    mctp_bus_set_eid(bus, TEST_EID_SOURCE);

    serial = mctp_serial_create();
    binding = mctp_serial_get_binding(serial);
    mctp_bus_transport_bind(bus, binding);

    REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_START);

    SECTION("Receive start frame flag: valid") {
        mctp_serial_byte_rx(binding, MCTP_SERIAL_FRAME_FLAG);
        REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_REVISION);

        SECTION("Receive revision: valid") {
            mctp_serial_byte_rx(binding, MCTP_SERIAL_REVISION);
            REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_PKT_LEN);

            SECTION("Receive byte count: valid") {
                mctp_serial_byte_rx(binding, TEST_BYTE_COUNT);
                REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_DATA);

                SECTION("Receive data: valid") {
                    // Receive packet overhead
                    for(size_t i = 0; i < TEST_PKT_HEADERS_SIZE; ++i)
                    {
                        mctp_serial_byte_rx(binding, TEST_FRAME_PACKET.DATA[i]);
                    }
                    REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_DATA);

                    uint8_t const *frame_payload = TEST_FRAME_PACKET.FIELDS.payload;
                    uint8_t const *escaped_payload = TEST_CRC_STRUCT.FIELDS.packet.payload;

                    // Receive "escape flag" (0x7D)
                    mctp_serial_byte_rx(binding, *(frame_payload++));
                    REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_ESCAPE);
                    mctp_serial_byte_rx(binding, *(frame_payload++));
                    REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_DATA);
                    REQUIRE(serial->rx.packet.io.data[serial->rx.next_pkt_byte - 1] == *(escaped_payload++));

                    // Receive "frame flag" (0x7E)
                    mctp_serial_byte_rx(binding, *(frame_payload++));
                    REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_ESCAPE);
                    mctp_serial_byte_rx(binding, *(frame_payload++));
                    REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_DATA);
                    REQUIRE(serial->rx.packet.io.data[serial->rx.next_pkt_byte - 1] == *(escaped_payload++));

                    // Receive regular data (also last data byte)
                    mctp_serial_byte_rx(binding, *(frame_payload++));
                    REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_FCS_HIGH);
                    REQUIRE(serial->rx.packet.io.data[serial->rx.next_pkt_byte - 1] == *(escaped_payload++));

                    const uint8_t* test_packet_data = TEST_CRC_STRUCT.FIELDS.packet.data;
                    REQUIRE(memcmp(serial->rx.packet.io.data, test_packet_data, TEST_BYTE_COUNT) == 0);

                    SECTION("Receive CRC: valid") {
                        mctp_serial_byte_rx(binding, MCTP_CRC16_GET_HIGH(TEST_CRC_VAL));
                        REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_FCS_LOW);

                        mctp_serial_byte_rx(binding, MCTP_CRC16_GET_LOW(TEST_CRC_VAL));
                        REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_END);

                        SECTION("Receive end frame flag: valid") {
                            mctp_serial_byte_rx(binding, MCTP_SERIAL_FRAME_FLAG);
                            REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_START);

                            REQUIRE_FALSE(mctp_pktq_empty(&bus->rx.packet_queue));
                        }

                        SECTION("Receive end frame flag: random byte") {
                            mctp_serial_byte_rx(binding, TEST_RANDOM_BYTE);
                            REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_START);

                            REQUIRE(mctp_pktq_empty(&bus->rx.packet_queue));
                        }
                    }

                    SECTION("Receive CRC: fail") {
                        mctp_serial_byte_rx(binding, 0);
                        REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_FCS_LOW);

                        mctp_serial_byte_rx(binding, 0);
                        REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_END);

                        mctp_serial_byte_rx(binding, MCTP_SERIAL_FRAME_FLAG);
                        REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_START);

                        REQUIRE(mctp_pktq_empty(&bus->rx.packet_queue));
                    }
                }

                SECTION("Receive escaped data: fail") {
                    mctp_serial_byte_rx(binding, MCTP_SERIAL_ESCAPE_FLAG);
                    REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_ESCAPE);

                    mctp_serial_byte_rx(binding, TEST_RANDOM_BYTE);
                    REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_START);
                }
            }

            SECTION("Receive byte count: < MIN") {
                mctp_serial_byte_rx(binding, MCTP_PKT_MIN_SIZE - 1);
                REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_START);             
            }

            SECTION("Receive byte count: > MAX") {
                mctp_serial_byte_rx(binding, MCTP_PKT_MAX_SIZE + 1);
                REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_START);             
            }
        }

        SECTION("Receive revision: frame flag") {
            mctp_serial_byte_rx(binding, MCTP_SERIAL_FRAME_FLAG);
            REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_REVISION); 
        }

        SECTION("Receive revision: random byte") {
            mctp_serial_byte_rx(binding, TEST_RANDOM_BYTE);
            REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_START);
        }
    }

    SECTION("Receive start frame flag: random byte") {
        mctp_serial_byte_rx(binding, TEST_RANDOM_BYTE);
        REQUIRE(serial->rx.state == MCTP_SERIAL_RX_STATE_SYNC_START);
    }

    // Clean up
    mctp_serial_destroy(serial);
    mctp_bus_destroy(bus);
}
