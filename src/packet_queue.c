#include <mctp/core/packet_queue.h>
#include <stdlib.h>
#include <string.h>


mctp_pktq_node_t *mctp_pktq_node_create(
    const mctp_transport_header_t *header,
    const uint8_t payload_data[],
    const size_t payload_len
) {
    mctp_pktq_node_t *node = (mctp_pktq_node_t*)calloc(1, sizeof(mctp_pktq_node_t));

    memcpy(&node->packet.header, header, sizeof(mctp_transport_header_t));
    memcpy(node->packet.payload, payload_data, payload_len);
    node->packet_len = payload_len + sizeof(mctp_transport_header_t);

    return node;
}

void mctp_pktq_node_destroy(
    mctp_pktq_node_t *node
) {
    free(node);
}

void mctp_pktq_push(
    mctp_pktq_t *queue,
    mctp_pktq_node_t *node
) {
    node->prev = queue->tail;
    queue->tail = node;

    while(queue->tail->prev != NULL) {
        queue->tail = queue->tail->prev;
    }

    if(queue->head == NULL)
    {
        queue->head = node;
    }
}

mctp_pktq_node_t *mctp_pktq_pop(
    mctp_pktq_t *queue
) {
    mctp_pktq_node_t *node = queue->head;
    queue->head = node->prev;

    if(queue->head == NULL)
    {
        queue->tail = NULL;
    }

    return node;
}

bool mctp_pktq_empty(
    mctp_pktq_t *queue
) {
    return (queue->head == NULL);
}
