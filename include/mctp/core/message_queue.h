#ifndef _MCTP_CORE_MESSAGE_QUEUE_H_
#define _MCTP_CORE_MESSAGE_QUEUE_H_

#include <mctp/util/queue/decl.h>
#include <mctp/core/packet.h>

typedef struct
{
    union
    {
        uint8_t *data;
        char *c_data;
    };
    
    size_t len;
    mctp_msg_ctx_t context;
}
mctp_message_t;


_x_queue_type_decl(
    mctp_msgq,
    mctp_message_t
)

#endif // _MCTP_CORE_MESSAGE_QUEUE_H_
