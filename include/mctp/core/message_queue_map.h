#ifndef _MCTP_CORE_MESSAGE_QUEUE_MAP_H_
#define _MCTP_CORE_MESSAGE_QUEUE_MAP_H_

#include <mctp/util/avl_tree/decl.h>
#include <mctp/core/base.h>
#include <mctp/core/packet_queue.h>


typedef struct
{
    mctp_pktq_t queue;
    mctp_msg_ctx_t context;
}
mctp_msgq_t;


_x_avl_tree_type_decl(
    mctp_msgq_map,
    mctp_msgq_t
)

#endif // _MCTP_CORE_MESSAGE_QUEUE_MAP_H_
