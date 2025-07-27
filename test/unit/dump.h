#ifndef _MCTP_TEST_UNIT_DUMP_H_
#define _MCTP_TEST_UNIT_DUMP_H_

#include <mctp/core/base.h>
#include <mctp/core/packet.h>
#include <mctp/binding/serial.h>
#include <mctp/control/message.h>


void mctp_generic_header_dump(
    const mctp_generic_header_t *header
);

void mctp_pkt_header_dump(
    const mctp_transport_header_t *header
);

void mctp_serial_header_dump(
    const mctp_serial_header_t *header
);

void mctp_serial_trailer_dump(
    const mctp_serial_trailer_t *trailer
);

void mctp_ctrl_header_dump(
    const mctp_ctrl_header_t *header
);

#endif // _MCTP_TEST_UNIT_DUMP_H_
