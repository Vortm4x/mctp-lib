#ifndef _MCTP_CORE_PACKET_QUEUE_H_
#define _MCTP_CORE_PACKET_QUEUE_H_

#include <mctp/util/queue/decl.h>
#include <mctp/util/forward_decl.h>

forward_decl(mctp_pkt_t);


_x_queue_type_decl(
    mctp_pktq,
    mctp_pkt_t *
)

#endif // _MCTP_CORE_PACKET_QUEUE_H_
