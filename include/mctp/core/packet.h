#ifndef _MCTP_CORE_PACKET_H_
#define _MCTP_CORE_PACKET_H_

#include <mctp/core/base.h>

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


typedef struct
{
    union __attribute__ ((__packed__))
    {
        struct __attribute__ ((__packed__))
        {
            mctp_transport_header_t header;
            uint8_t payload[MCTP_BASE_MTU];
        };
        uint8_t data[sizeof(mctp_transport_header_t) + MCTP_BASE_MTU];
    };
    size_t len;
}
mctp_packet_t;


mctp_packet_t *mctp_pkt_create(
    const mctp_transport_header_t *header,
    const uint8_t payload_data[],
    const size_t payload_len
);

void mctp_pkt_destroy(
    mctp_packet_t *packet
);

#endif // _MCTP_CORE_PACKET_H_