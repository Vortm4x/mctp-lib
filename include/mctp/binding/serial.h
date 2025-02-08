#ifndef _MCTP_BINDING_SERIAL_H_
#define _MCTP_BINDING_SERIAL_H_

#include <mctp/core/binding.h>
#include <mctp/core/packet.h>

#define MCTP_SERIAL_REVISION        0x01
#define MCTP_SERIAL_FRAME_FLAG      0x7E
#define MCTP_SERIAL_ESCAPE_FLAG     0x7D

#define MCTP_SERIAL_ESCAPE_BYTE(byte) (byte ^ 0x20)


typedef enum mctp_serial_rx_state_t
{
    MCTP_SERIAL_RX_STATE_SYNC_START,
	MCTP_SERIAL_RX_STATE_REVISION,
    MCTP_SERIAL_RX_STATE_PKT_LEN,
	MCTP_SERIAL_RX_STATE_DATA,
	MCTP_SERIAL_RX_STATE_ESCAPE,
	MCTP_SERIAL_RX_STATE_FCS_HIGH,
	MCTP_SERIAL_RX_STATE_FCS_LOW,
	MCTP_SERIAL_RX_STATE_SYNC_END,
}
mctp_serial_rx_state_t;


typedef void (*mctp_serial_byte_tx_t)(
    const uint8_t byte
);

typedef struct
{
    mctp_binding_t binding;
    mctp_serial_byte_tx_t byte_tx;

    struct
    {
        mctp_packet_t packet;
        size_t next_pkt_byte;
        mctp_serial_rx_state_t state;
        uint16_t fcs_calc;
        uint16_t fcs_read;
    }
    rx;
}
mctp_serial_t;

typedef union __attribute__ ((__packed__))
{
    struct __attribute__ ((__packed__))
    {
        uint8_t framing_flag;
        uint8_t revision;
        uint8_t byte_count;
    };
    uint8_t data[3];
}
mctp_serial_header_t;

typedef union __attribute__ ((__packed__))
{
    struct __attribute__ ((__packed__))
    {
        uint8_t fcs_high;
        uint8_t fcs_low;
        uint8_t framing_flag;
    };
    uint8_t data[3];
}
mctp_serial_trailer_t;


mctp_serial_t* mctp_serial_create();

void mctp_serial_destroy(
    mctp_serial_t *serial
);

void mctp_serial_set_byte_tx(
    mctp_serial_t *serial,
    mctp_serial_byte_tx_t byte_tx
);

mctp_binding_t *mctp_serial_get_binding(
    mctp_serial_t *serial
);

void mctp_serial_buffer_tx(
    const mctp_serial_t *serial,
    const uint8_t buffer_data[],
    const size_t buffer_len
);

void mctp_serial_escaped_buffer_tx(
    const mctp_serial_t *serial,
    const uint8_t buffer_data[],
    const size_t buffer_len
);

void mctp_serial_frame_tx(
    const mctp_serial_t *serial,
    const mctp_serial_header_t *header,
	const mctp_packet_t *packet,
    const mctp_serial_trailer_t *trailer
);

void mctp_serial_packet_tx(
    const mctp_binding_t *binding,
	const mctp_packet_t *packet
);

void mctp_serial_reset_rx_ctx(
    mctp_serial_t* serial
);

void mctp_serial_push_rx_data(
    mctp_serial_t *serial,
	const uint8_t byte
);

void mctp_serial_byte_rx(
    const mctp_binding_t *binding,
	const uint8_t byte
);

#endif // _MCTP_BINDING_SERIAL_H_
