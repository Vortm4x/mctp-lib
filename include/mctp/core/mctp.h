#ifndef _MCTP_CORE_MCTP_H_
#define _MCTP_CORE_MCTP_H_

#include <mctp/core/bus.h>

#define MCTP_PKT_HDR_VER 0b0001


typedef struct
{
    const mctp_eid_t eid;
    uint8_t message_tag     : 3;
    bool tag_owner          : 1;
}
mctp_msg_ctx_t;


uint8_t mctp_get_message_tag();

mctp_pktq_node_t* mctp_message_disassemble(
    const mctp_eid_t source,
    const mctp_msg_ctx_t *message_ctx,
    const uint8_t message_data[],
    const size_t message_len
);

void mctp_message_tx(
    const mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx,
    const uint8_t message_data[],
    const size_t message_len
);


#endif // _MCTP_CORE_MCTP_H_ 
