#ifndef _MCTP_CORE_P_MESSAGE_H_
#define _MCTP_CORE_P_MESSAGE_H_

#include <mctp/core/message.h>
#include <mctp/util/packed.h>

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

#endif // _MCTP_CORE_P_MESSAGE_H_
