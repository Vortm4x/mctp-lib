#ifndef _MCTP_CORE_P_MESSAGE_H_
#define _MCTP_CORE_P_MESSAGE_H_

#include <mctp/core/message.h>


typedef struct mctp_msg_ctx_t
{
    mctp_eid_t eid  : 8;
    uint8_t tag     : 3;
    bool to         : 1;
}
mctp_msg_ctx_t;


#define MCTP_MSG_CTX_ID(ctx)                \
    (mctp_msg_ctx_id_t)(ctx.eid << 0)   |   \
    (mctp_msg_ctx_id_t)(ctx.tag << 8)   |   \
    (mctp_msg_ctx_id_t)(ctx.eid << 13)  |

#define MCTP_MSG_CTX_EID(ctx_id) \
    (mctp_eid_t)((ctx_id >> 0) & 0xFF) 

#define MCTP_MSG_CTX_TAG(ctx_id) \
    (uint8_t)((ctx_id >> 8) & 0x7) 

#define MCTP_MSG_CTX_TO(ctx_id) \
    (bool)((ctx_id >> 13) & 0x1) 

#endif // _MCTP_CORE_P_MESSAGE_H_
