#ifndef _MCTP_BINDING_SERIAL_H_
#define _MCTP_BINDING_SERIAL_H_

#include <mctp/core/binding.h>
#include <mctp/core/packet.h>

#define MCTP_SERIAL_REVISION        0x01
#define MCTP_SERIAL_FRAME_FLAG      0x7E


typedef void (*mctp_serial_buffer_tx_t)(
    const uint8_t buffer_data[],
    const size_t buffer_len
);

typedef struct
{
    mctp_binding_t binding;
    mctp_serial_buffer_tx_t buffer_tx;
}
mctp_serial_t;

typedef struct __attribute__ ((__packed__))
{
    uint8_t framing_flag;
    uint8_t revision;
    uint8_t byte_count;
}
mctp_serial_header_t;

typedef struct __attribute__ ((__packed__))
{
    uint8_t fcs_high;
    uint8_t fcs_low;
    uint8_t framing_flag;
}
mctp_serial_trailer_t;

typedef struct
{
    mctp_serial_header_t *header;
    uint8_t *packet;
    mctp_serial_trailer_t *trailer;
}
mctp_serial_buffer_map_t;


mctp_serial_t* mctp_serial_create();

void mctp_serial_destroy(
    mctp_serial_t *serial
);

void mctp_serial_set_buffer_tx(
    mctp_serial_t *serial,
    mctp_serial_buffer_tx_t buffer_tx
);

mctp_binding_t *mctp_serial_get_binding(
    const mctp_serial_t *serial
);

void mctp_serial_buffer_map_init(
    mctp_serial_buffer_map_t *buffer_map,
    uint8_t *buffer_data,
    const size_t packet_len
);

void mctp_serial_packet_tx(
    const mctp_binding_t *binding,
	const mctp_packet_t *packet
);

#endif // _MCTP_BINDING_SERIAL_H_
