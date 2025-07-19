#include <mctp/core/mctp.h>
#include <mctp/core/binding.h>


uint8_t mctp_get_message_tag()
{
    static uint8_t msg_tag = 0;

    return msg_tag++;
}

void mctp_message_disassemble(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx,
    const uint8_t message_data[],
    const size_t message_len
) {
    mctp_transport_header_t header = {
        .version = MCTP_PKT_HDR_VER,
        .dest = message_ctx->eid,
        .source = bus->eid,
        .tag = message_ctx->tag,
        .tag_owner = message_ctx->tag_owner,
    };

    const size_t packet_count = 
        (message_len / MCTP_BASE_MTU) +
        (message_len % MCTP_BASE_MTU ? 1 : 0);

    for(size_t i = 0; i < packet_count; ++i)
    {
        header.som = (i == 0);
        header.eom = (i + 1 == packet_count);

        const uint8_t *payload_data = &message_data[MCTP_BASE_MTU * i];
        const size_t payload_len = header.eom 
            ? message_len - MCTP_BASE_MTU * i
            : MCTP_BASE_MTU;

        mctp_packet_t *packet = mctp_pkt_create(
            &header,
            payload_data,
            payload_len
        );

        mctp_pktq_enqueue(tx_queue, packet);

        header.pkt_seq++;
    }
}

void mctp_pktq_drain(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus
) {
    while (!mctp_pktq_empty(tx_queue))
    {
        mctp_packet_t *packet = mctp_pktq_dequeue(tx_queue);

        mctp_packet_tx(bus, packet);
        mctp_pkt_destroy(packet);
    }
}

void mctp_pktq_tx(
    const mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus
) {
    mctp_pktq_node_t *node = mctp_pktq_front(tx_queue);

    while (node != NULL)
    {
        mctp_packet_t *packet = mctp_pktq_node_value(node);

        mctp_packet_tx(bus, packet);
        node = mctp_pktq_node_next(node);
    }
}

void mctp_packet_tx(
    const mctp_bus_t *bus,
    const mctp_packet_t *packet
) {
    const mctp_binding_t *binding = bus->binding;

    binding->packet_tx(binding, packet);
}

void mctp_packet_rx(
    mctp_bus_t *bus,
    const mctp_packet_t *packet
) {
    mctp_packet_t *rx_packet = mctp_pkt_clone(packet);

    mctp_pktq_enqueue(&bus->rx.packet_queue, rx_packet);

    if(rx_packet->io.header.eom)
    {
        mctp_msg_ctx_t message_ctx = {
            .eid = rx_packet->io.header.source,
            .tag = rx_packet->io.header.tag,
            .tag_owner = rx_packet->io.header.tag_owner
        };

        mctp_msgq_enqueue(
            &bus->rx.message_queue,
            message_ctx
        );
    }
}

void mctp_pktq_rx(
    mctp_bus_t *bus,
    mctp_pktq_t *rx_queue,
    const mctp_msg_ctx_t *message_ctx
) {
    mctp_pktq_t temp_queue = {};

    while (!mctp_pktq_empty(&bus->rx.packet_queue))
    {
        mctp_packet_t *curr = mctp_pktq_dequeue(&bus->rx.packet_queue);

        if(mctp_pkt_message_match(curr, message_ctx))
        {
            mctp_pktq_enqueue(rx_queue, curr);
        }
        else
        {
            mctp_pktq_enqueue(&temp_queue, curr);
        }
    }

    if(!mctp_pktq_empty(&temp_queue))
    {
        temp_queue.rear->next = bus->rx.packet_queue.front;
        bus->rx.packet_queue.front = temp_queue.front;
    }
}

void mctp_generic_header_dump(
    const mctp_generic_header_t *header
) {
    printf("MCTP Generic header\n");
    printf("integrity_check:    %s\n",      header->integrity_check ? "YES" : "NO");
    printf("type:               0x%02X\n",  header->type);
}
