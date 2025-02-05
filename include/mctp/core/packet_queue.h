#ifndef _MCTP_CORE_PACKET_QUEUE_H_
#define _MCTP_CORE_PACKET_QUEUE_H_

#include <mctp/core/packet.h>


typedef struct mctp_pktq_node_t
{
    mctp_packet_t *packet;
    struct mctp_pktq_node_t *next;
}
mctp_pktq_node_t;

typedef struct
{
    mctp_pktq_node_t *front;
    mctp_pktq_node_t *rear;
}
mctp_pktq_t;


mctp_pktq_node_t *mctp_pktq_node_create(
    mctp_packet_t *packet
);

void mctp_pktq_node_destroy(
    mctp_pktq_node_t *node
);

mctp_pktq_node_t *mctp_pktq_node_next(
    const mctp_pktq_node_t *node
);

mctp_packet_t *mctp_pktq_node_pkt(
    const mctp_pktq_node_t *node
);

void mctp_pktq_enqueue(
    mctp_pktq_t *queue,
    mctp_packet_t *packet
);

mctp_packet_t *mctp_pktq_dequeue(
    mctp_pktq_t *queue
);

mctp_pktq_node_t *mctp_pktq_front(
    const mctp_pktq_t *queue
);

bool mctp_pktq_empty(
    const mctp_pktq_t *queue
);

#endif // _MCTP_CORE_PACKET_QUEUE_H_