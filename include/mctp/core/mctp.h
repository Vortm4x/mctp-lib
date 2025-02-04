#ifndef _MCTP_CORE_MCTP_H_
#define _MCTP_CORE_MCTP_H_

#include <mctp/core/bus.h>
#include <stddef.h>

#define MCTP_PKT_HDR_VER 0b0001



typedef struct mctp_msg_ctx_t
{
    const mctp_eid_t eid;
    uint8_t message_tag     : 3;
    bool tag_owner          : 1;
}
mctp_msg_ctx_t;

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



uint8_t mctp_get_message_tag();

void mctp_message_tx(
    const mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx,
    const uint8_t message_data[],
    const size_t message_len
);


#endif // _MCTP_CORE_MCTP_H_ 
