#ifndef _MCTP_CORE_MESSAGE_QUEUE_MAP_H_
#define _MCTP_CORE_MESSAGE_QUEUE_MAP_H_

#include <mctp/util/avl_tree/decl.h>
#include <mctp/core/base.h>
#include <mctp/core/packet_queue.h>


_x_avl_tree_type_decl(
    mctp_msgq_map,
    mctp_pktq_t,
    mctp_msg_ctx_t
)

#endif // _MCTP_CORE_MESSAGE_QUEUE_MAP_H_
