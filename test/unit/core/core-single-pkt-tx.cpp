#include <catch2/catch_test_macros.hpp>

#include <mctp/control/message.h>
#include <mctp/core/bus.h>


constexpr mctp_eid_t TEST_EID_SOURCE    = 0xA;
constexpr mctp_eid_t TEST_EID_DEST      = 0xB;
constexpr size_t TEST_PKT_PAYLOAD_LEN   = MCTP_BASE_MTU;

constexpr uint8_t TEST_PKT_SEQ = 0;

constexpr uint8_t TEST_PKT_PAYLOAD_DATA[TEST_PKT_PAYLOAD_LEN] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F
};


static void fake_binding_packet_tx(
    const struct mctp_binding_t *binding,
	const mctp_packet_t *packet
) {
    const size_t payload_len = mctp_pkt_payload_len(packet);
    REQUIRE(payload_len == TEST_PKT_PAYLOAD_LEN);

    const mctp_transport_header_t *hdr = &packet->io.header;

    mctp_pkt_header_dump(&packet->io.header);
    REQUIRE(hdr->version    == MCTP_PKT_HDR_VER);
    REQUIRE(hdr->dest       == TEST_EID_DEST);
    REQUIRE(hdr->source     == TEST_EID_SOURCE);
    REQUIRE(hdr->som        == true);
    REQUIRE(hdr->eom        == true);
    REQUIRE(hdr->pkt_seq    == TEST_PKT_SEQ);
    REQUIRE(hdr->tag_owner  == true);

    REQUIRE(memcmp(packet->io.payload, TEST_PKT_PAYLOAD_DATA, payload_len) == 0);
}


TEST_CASE("core-single-pkt-tx") {
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
        TEST_PKT_PAYLOAD_DATA,
        TEST_PKT_PAYLOAD_LEN
    );

    // Drain tx queue
    mctp_pktq_drain(&tx_queue, bus);

    // Clean up
    mctp_bus_destroy(bus);
}
