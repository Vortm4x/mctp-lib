#ifndef _MCTP_CORE_PACKET_H_
#define _MCTP_CORE_PACKET_H_

#include <mctp/core/base.h>

#define MCTP_BASE_MTU 64


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

typedef union __attribute__ ((__packed__))
{
    struct __attribute__ ((__packed__))
    {
        mctp_transport_header_t header;
        uint8_t payload[MCTP_BASE_MTU];
    };
    uint8_t data[sizeof(mctp_transport_header_t) + MCTP_BASE_MTU];
}
mctp_packet_t;

#endif // _MCTP_CORE_PACKET_H_