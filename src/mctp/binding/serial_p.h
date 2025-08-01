#ifndef _MCTP_BINDING_SERIAL_P_H_
#define _MCTP_BINDING_SERIAL_P_H_

#include <mctp/binding/serial.h>
#include <mctp/core/binding_p.h>
#include <mctp/core/packet_p.h>
#include <mctp/util/packed.h>


#define MCTP_SERIAL_REVISION        ((uint8_t)0x01)
#define MCTP_SERIAL_FRAME_FLAG      ((uint8_t)0x7E)
#define MCTP_SERIAL_ESCAPE_FLAG     ((uint8_t)0x7D)

#define MCTP_SERIAL_ESCAPE_BYTE(byte) (byte ^ ((uint8_t)0x20))


typedef packed_union
{
    packed_struct
    {
        uint8_t frame_flag;
        uint8_t revision;
        uint8_t byte_count;
    };
    uint8_t data[3];
}
mctp_serial_hdr_t;

static_assert(sizeof(mctp_serial_hdr_t) == 3);


typedef packed_union
{
    packed_struct
    {
        uint8_t fcs_high;
        uint8_t fcs_low;
        uint8_t frame_flag;
    };
    uint8_t data[3];
}
mctp_serial_trail_t;

static_assert(sizeof(mctp_serial_trail_t) == 3);


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


typedef struct mctp_serial_t
{
    mctp_binding_t binding;
    mctp_serial_byte_tx_t byte_tx;

    struct
    {
        mctp_pkt_t packet;
        size_t next_pkt_byte;
        mctp_serial_rx_state_t state;
        uint16_t fcs_calc;
        uint16_t fcs_read;
    }
    rx;
}
mctp_serial_t;

#endif // _MCTP_BINDING_SERIAL_P_H_
