#ifndef _MCTP_CORE_PACKET_QUEUE_H_
#define _MCTP_CORE_PACKET_QUEUE_H_

#include <mctp/util/queue_decl.h>
#include <mctp/core/packet.h>


_queue_type_decl(
    mctp_pktq,
    mctp_packet_t *
)

#endif // _MCTP_CORE_PACKET_QUEUE_H_