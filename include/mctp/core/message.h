#ifndef _MCTP_CORE_MESSAGE_H_
#define _MCTP_CORE_MESSAGE_H_

#include <mctp/core/base.h>
#include <mctp/util/forward_decl.h>
#include <mctp/util/extern_c.h>

forward_decl(mctp_pktq_t);
forward_decl(mctp_bus_t);


#define MCTP_TAG_OWNER_SEND   true
#define MCTP_TAG_OWNER_REPLY  false

typedef struct mctp_msg_ctx_t
{
    mctp_eid_t remote_eid;
    uint8_t tag : 3;
    bool tag_owner : 1;
}
mctp_msg_ctx_t;


EXTERN_C_BEGIN

void mctp_fill_send_queue(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const uint8_t *msg_data,
    const size_t msg_len,
    const mctp_eid_t dest
);

void mctp_fill_reply_queue(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const uint8_t *msg_data,
    const size_t msg_len,
    const mctp_eid_t dest,
    const uint8_t msg_tag
);

void mctp_message_disassemble(
    mctp_pktq_t *tx_queue,
    const uint8_t *msg_data,
    const size_t msg_len,
    const mctp_eid_t source,
    const mctp_eid_t dest,
    const uint8_t msg_tag,
    const bool tag_owner
);

EXTERN_C_END

#endif // _MCTP_CORE_MESSAGE_H_
