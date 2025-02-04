#ifndef _MCTP_CORE_PACKET_QUEUE_H_
#define _MCTP_CORE_PACKET_QUEUE_H_

#include <mctp/core/packet.h>


typedef struct mctp_pktq_node_t
{
    mctp_packet_t packet;
    size_t packet_len;
    struct mctp_pktq_node_t *prev;
}
mctp_pktq_node_t;

typedef struct
{
    mctp_pktq_node_t *head;
    mctp_pktq_node_t *tail;
}
mctp_pktq_t;


mctp_pktq_node_t *mctp_pktq_node_create(
    const mctp_transport_header_t *header,
    const uint8_t payload_data[],
    const size_t payload_len
);

void mctp_pktq_node_destroy(
    mctp_pktq_node_t *node
);

void mctp_pktq_push(
    mctp_pktq_t *queue,
    mctp_pktq_node_t *node
);

mctp_pktq_node_t *mctp_pktq_pop(
    mctp_pktq_t *queue
);

bool mctp_pktq_empty(
    mctp_pktq_t *queue
);

#endif // _MCTP_CORE_PACKET_QUEUE_H_