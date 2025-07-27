#include <catch2/catch_test_macros.hpp>
#include <unit/dump.h>

#include <mctp/control/message.h>
#include <mctp/core/bus.h>


constexpr mctp_eid_t TEST_EID_SOURCE    = 0xA;
constexpr mctp_eid_t TEST_EID_DEST      = 0xB;

constexpr uint8_t TEST_PKT_SEQ_SOM = 0;
constexpr uint8_t TEST_PKT_SEQ_MID = 1;
constexpr uint8_t TEST_PKT_SEQ_EOM = 2;
constexpr uint8_t TEST_PKT_COUNT = 3;

constexpr size_t TEST_PKT_PAYLOAD_LEN[TEST_PKT_COUNT] = {
    MCTP_BASE_MTU,
    MCTP_BASE_MTU,
    MCTP_BASE_MTU / 2
};

constexpr size_t TEST_PAYLOAD_LEN
    = TEST_PKT_PAYLOAD_LEN[0]
    + TEST_PKT_PAYLOAD_LEN[1]
    + TEST_PKT_PAYLOAD_LEN[2];

constexpr uint8_t TEST_PAYLOAD_DATA[TEST_PAYLOAD_LEN] = {
    // pkt_seq = 0 (64 bytes)
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    // pkt_seq = 1 (64 bytes)
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    // pkt_seq = 2 (32 bytes)
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
};


static void fake_binding_packet_tx(
    const struct mctp_binding_t *binding,
	const mctp_packet_t *packet
) {
    const size_t payload_len = mctp_pkt_payload_len(packet);
    REQUIRE(payload_len > 0);

    const mctp_transport_header_t *hdr = &packet->io.header;

    mctp_pkt_header_dump(&packet->io.header);

    REQUIRE(hdr->pkt_seq < TEST_PKT_COUNT);

    REQUIRE(payload_len == TEST_PKT_PAYLOAD_LEN[hdr->pkt_seq]);

    if(hdr->pkt_seq == TEST_PKT_SEQ_SOM) {
        REQUIRE(hdr->som        == true);
        REQUIRE(hdr->eom        == false);
    }
    if(hdr->pkt_seq == TEST_PKT_SEQ_MID) {
        REQUIRE(hdr->som        == false);
        REQUIRE(hdr->eom        == false);
    }
    if(hdr->pkt_seq == TEST_PKT_SEQ_EOM) {
        REQUIRE(hdr->som        == false);
        REQUIRE(hdr->eom        == true);
    }

    REQUIRE(hdr->version    == MCTP_PKT_HDR_VER);
    REQUIRE(hdr->dest       == TEST_EID_DEST);
    REQUIRE(hdr->source     == TEST_EID_SOURCE);
    REQUIRE(hdr->tag_owner  == true);

    const size_t packet_offset = hdr->pkt_seq * MCTP_BASE_MTU;
    const uint8_t *packet_data = &TEST_PAYLOAD_DATA[packet_offset];

    REQUIRE(memcmp(packet->io.payload, packet_data, payload_len) == 0);
}


TEST_CASE("core-multi-pkt-tx") {
    mctp_bus_t *bus = NULL;
    mctp_pktq_t tx_queue = {};
    mctp_binding_t fake_binding = {
        .packet_tx = fake_binding_packet_tx
    };

    // Bus setup
    bus = mctp_bus_create();
    mctp_bus_set_eid(bus, TEST_EID_SOURCE);
    mctp_bus_transport_bind(bus, &fake_binding);

    // Message setup
    const mctp_msg_ctx_t message_ctx = {
        .eid = TEST_EID_DEST,
        .tag = mctp_get_message_tag(),
        .tag_owner = true
    };

    // Fill tx queue
    mctp_message_disassemble(
        &tx_queue,
        bus,
        &message_ctx,
        TEST_PAYLOAD_DATA,
        TEST_PAYLOAD_LEN
    );

    // Drain tx queue
    mctp_pktq_drain(&tx_queue, bus);

    // Clean up
    mctp_bus_destroy(bus);
}
