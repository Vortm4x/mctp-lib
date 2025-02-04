#include <mctp/core/packet_queue.h>
#include <stdlib.h>


mctp_pktq_node_t *mctp_pktq_node_create(
    mctp_packet_t *packet
) {
    mctp_pktq_node_t *node = (mctp_pktq_node_t*)calloc(1, sizeof(mctp_pktq_node_t));

    node->packet = packet;

    return node;
}

void mctp_pktq_node_destroy(
    mctp_pktq_node_t *node
) {
    free(node);
}

void mctp_pktq_enqueue(
    mctp_pktq_t *queue,
    mctp_packet_t *packet
) {
    mctp_pktq_node_t *node = mctp_pktq_node_create(packet);

    if(mctp_pktq_empty(queue))
    {
        queue->front = node;
    }
    else
    {
        queue->rear->next = node;
    }

    queue->rear = node;
}


mctp_packet_t *mctp_pktq_dequeue(
    mctp_pktq_t *queue
) {
    mctp_packet_t *packet = NULL;

    if(!mctp_pktq_empty(queue))
    {
        mctp_pktq_node_t *node = queue->front;
        packet = node->packet;

        queue->front = queue->front->next;

        if(mctp_pktq_empty(queue))
        {
            queue->rear = NULL;
        }

        mctp_pktq_node_destroy(node);
    }

    return packet;
}

bool mctp_pktq_empty(
    mctp_pktq_t *queue
) {
    return (queue->front == NULL);
}
