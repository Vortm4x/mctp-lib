#include <mctp/core/packet.h>
#include <mctp/binding/serial.h>

void pkt_header_dump(
    const mctp_transport_header_t *header
);

void serial_header_dump(
    const mctp_serial_header_t *header
);

void serial_trailer_dump(
    const mctp_serial_trailer_t *trailer
);
