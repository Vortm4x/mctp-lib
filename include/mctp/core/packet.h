#ifndef _MCTP_CORE_PACKET_H_
#define _MCTP_CORE_PACKET_H_

#include <mctp/core/base.h>
#include <mctp/util/extern_c.h>

typedef struct __attribute__ ((__packed__))
{
    uint8_t version     : 4;
    uint8_t             : 4;
    mctp_eid_t dest;
    mctp_eid_t source;
    uint8_t tag         : 3;
    bool tag_owner      : 1;
    uint8_t pkt_seq     : 2;
    bool eom            : 1;
    bool som            : 1;
}
mctp_transport_header_t;

#define MCTP_PKT_HDR_SIZE (sizeof(mctp_transport_header_t))
#define MCTP_PKT_MAX_SIZE (MCTP_PKT_HDR_SIZE + MCTP_BASE_MTU)
#define MCTP_PKT_MIN_SIZE (MCTP_PKT_HDR_SIZE)

typedef union __attribute__ ((__packed__))
{
    struct __attribute__ ((__packed__))
    {
        mctp_transport_header_t header;
        uint8_t payload[MCTP_BASE_MTU];
    };
    uint8_t data[MCTP_PKT_MAX_SIZE];
}
mctp_io_packet_t;

typedef struct
{
    mctp_io_packet_t io;
    size_t len;
}
mctp_packet_t;


EXTERN_C_BEGIN

mctp_packet_t *mctp_pkt_create(
    const mctp_transport_header_t *header,
    const uint8_t payload_data[],
    const size_t payload_len
);

mctp_packet_t *mctp_pkt_clone(
    const mctp_packet_t *packet
);

void mctp_pkt_destroy(
    mctp_packet_t *packet
);

size_t mctp_pkt_payload_len(
    const mctp_packet_t *packet
);

EXTERN_C_END

#endif // _MCTP_CORE_PACKET_H_