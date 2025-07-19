#include <mctp/core/packet.h>
#include <mctp/binding/serial.h>


template<class test_io_struct_t>
struct test_io_struct_wrapper_t
{
    union
    {
        test_io_struct_t fields;
        uint8_t data[sizeof(test_io_struct_t)];
    };
};

template<size_t payload_size>
struct __attribute__ ((__packed__)) test_crc_struct_t
{
    uint8_t revision = MCTP_SERIAL_REVISION;
    uint8_t byte_count = payload_size + MCTP_PKT_HDR_SIZE;

    union {
        struct __attribute__ ((__packed__)) {
            mctp_transport_header_t transport_header;
            uint8_t payload[payload_size];
        };
        uint8_t data[payload_size + MCTP_PKT_HDR_SIZE];
    }
    packet;
};

template<size_t escaped_payload_size>
struct __attribute__ ((__packed__)) test_framed_packet_t
{
    mctp_transport_header_t transport_header;
    uint8_t payload[escaped_payload_size];
};

template<size_t escaped_payload_size>
struct __attribute__ ((__packed__)) test_serial_frame_t
{
    mctp_serial_header_t serial_header;
    test_framed_packet_t<escaped_payload_size> packet;
    mctp_serial_trailer_t serial_trailer;
};
