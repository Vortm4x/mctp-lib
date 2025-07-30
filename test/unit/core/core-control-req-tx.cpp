#include <catch2/catch_test_macros.hpp>
#include <unit/dump.h>

#include <mctp/control/message.h>
#include <mctp/control/request.h>
#include <mctp/core/bus.h>


constexpr mctp_eid_t TEST_EID_SOURCE    = 0xA;
constexpr mctp_eid_t TEST_EID_DEST      = 0xB;

constexpr size_t TEST_PKT_PAYLOAD_LEN   = MCTP_BASE_MTU;
constexpr size_t TEST_REQ_PAYLOAD_LEN   = TEST_PKT_PAYLOAD_LEN - MCTP_CTRL_HDR_SIZE;

constexpr uint8_t TEST_PKT_SEQ = 0;
constexpr bool TEST_NOT_DATAGRAM  = false;

constexpr mctp_ctrl_cmd_t TEST_CTRL_CMD = (mctp_ctrl_cmd_t)0;
constexpr uint8_t TEST_REQ_PAYLOAD_DATA[TEST_REQ_PAYLOAD_LEN] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C
};


typedef struct __attribute__ ((__packed__))
{
    mctp_ctrl_header_t header;

    MCTP_CTRL_REQ_LAYOUT
    (
        uint8_t data[TEST_REQ_PAYLOAD_LEN];
    )
    payload;
}
ctrl_req_mock_t;


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

    const ctrl_req_mock_t *req = (const ctrl_req_mock_t *)packet->io.payload;

    mctp_generic_header_dump(&req->header.base);
    REQUIRE(req->header.base.integrity_check    == false);
    REQUIRE(req->header.base.type               == MCTP_MSG_TYPE_CONTROL);

    mctp_ctrl_header_dump(&req->header);
    REQUIRE(req->header.command     == TEST_CTRL_CMD);
    REQUIRE(req->header.request     == true);
    REQUIRE(req->header.datagram    == TEST_NOT_DATAGRAM);

    REQUIRE(memcmp(req->payload.data, TEST_REQ_PAYLOAD_DATA, TEST_REQ_PAYLOAD_LEN) == 0);
}


TEST_CASE("core-control-req-tx") {
    mctp_bus_t *bus = NULL;
    mctp_pktq_t tx_queue = {};
    mctp_binding_t fake_binding = {
        .packet_tx = fake_binding_packet_tx
    };

    // Bus setup
    bus = mctp_bus_create();
    mctp_bus_set_eid(bus, TEST_EID_SOURCE);
    mctp_bus_transport_bind(bus, &fake_binding);

    // Fill tx queue
    mctp_ctrl_request_prepare(
        &tx_queue,
        bus,
        TEST_EID_DEST,
        TEST_CTRL_CMD,
        TEST_NOT_DATAGRAM,
        TEST_REQ_PAYLOAD_DATA,
        TEST_REQ_PAYLOAD_LEN
    );

    // Drain tx queue
    mctp_pktq_drain(&tx_queue, bus);

    // Clean up
    mctp_bus_destroy(bus);
}
