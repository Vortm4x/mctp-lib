#ifndef _MCTP_CORE_P_MESSAGE_H_
#define _MCTP_CORE_P_MESSAGE_H_

#include <mctp/core/message.h>

typedef uint16_t mctp_msg_ctx_raw_t;

#define MCTP_MSG_CTX_REMOTE_EID(raw) \
    (mctp_eid_t)((raw >> 0) & 0xFF)

#define MCTP_MSG_CTX_TAG(raw) \
    (uint8_t)((raw >> 8) & 0x3)

#define MCTP_MSG_CTX_TAG_OWNER(raw) \
    (bool)((raw >> 11) & 1)

#define MCTP_MSG_CTX_RAW(ctx)                   \
    (mctp_msg_ctx_raw_t)(ctx.remote_eid << 0)   \
  | (mctp_msg_ctx_raw_t)(ctx.tag        << 8)   \
  | (mctp_msg_ctx_raw_t)(ctx.tag_owner  << 11)


EXTERN_C_BEGIN

void mctp_message_disassemble(
    mctp_pktq_t *tx_queue,
    const uint8_t *msg_data,
    const size_t msg_len,
    const mctp_eid_t source,
    const mctp_eid_t dest,
    const uint8_t msg_tag,
    const bool tag_owner
);

void mctp_message_assemble(
    const mctp_pktq_t *rx_queue,
    uint8_t **message_data,
    size_t *message_len
);

EXTERN_C_END

#endif // _MCTP_CORE_P_MESSAGE_H_
