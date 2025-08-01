#include <mctp/core/mctp.h>
#include <mctp/core/binding.h>
#include <mctp/core/packet_queue_map.h>
#include <stdio.h>


static void mctp_msgq_update(
    mctp_bus_t *bus,
    const mctp_pktq_t *rx_queue,
    const mctp_msg_ctx_t *message_ctx
);

static mctp_pktq_t mctp_get_rx_queue(
    mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx
);

static void mctp_drop_rx_queue(
    mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx
);

static void mctp_push_rx_queue(
    mctp_bus_t *bus,
    const mctp_pktq_t *rx_queue,
    const mctp_msg_ctx_t *message_ctx
);


uint8_t mctp_get_message_tag()
{
    static uint8_t msg_tag = 0;

    return msg_tag++;
}

void mctp_message_disassemble(
    const mctp_bus_t *bus,
    const mctp_message_t *message,
    mctp_pktq_t *tx_queue
) {
    mctp_transport_header_t header = {
        .version = MCTP_PKT_HDR_VER,
        .dest = message->context.eid,
        .source = bus->eid,
        .tag = message->context.tag,
        .tag_owner = message->context.tag_owner,
    };

    const size_t packet_count =
        (message->len / MCTP_BASE_MTU) +
        (message->len % MCTP_BASE_MTU ? 1 : 0);

    for(size_t i = 0; i < packet_count; ++i)
    {
        header.som = (i == 0);
        header.eom = (i + 1 == packet_count);

        const uint8_t *payload_data = &message->data[MCTP_BASE_MTU * i];
        const size_t payload_len = header.eom
            ? message->len - MCTP_BASE_MTU * i
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

void mctp_message_assemble(
    mctp_message_t *message,
    const mctp_pktq_t *rx_queue
) {
    FILE *message_stream = open_memstream(
        &message->c_data,
        &message->len
    );

    mctp_pktq_node_t *node = mctp_pktq_front(rx_queue);

    while (node != NULL)
    {
        const mctp_packet_t *packet = mctp_pktq_node_data(node);

        fwrite(
            packet->io.payload,
            mctp_pkt_payload_len(packet), 1,
            message_stream
        );

        node = mctp_pktq_node_next(node);
    }

    fclose(message_stream);
}

void mctp_pktq_drain(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus
) {
    while (!mctp_pktq_empty(tx_queue))
    {
        mctp_packet_t *packet = mctp_pktq_node_data(
            mctp_pktq_front(tx_queue)
        );

        mctp_packet_tx(bus, packet);
        mctp_pktq_dequeue(tx_queue);
    }
}

void mctp_pktq_tx(
    const mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus
) {
    mctp_pktq_node_t *node = mctp_pktq_front(tx_queue);

    while (node != NULL)
    {
        mctp_packet_t *packet = mctp_pktq_node_data(node);

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
    const mctp_transport_header_t *rx_header = &packet->io.header;

    if (rx_header->version  != MCTP_PKT_HDR_VER
     || rx_header->dest     != bus->eid)
    {
        return;
    }

    mctp_msg_ctx_t message_ctx = {
        .eid = rx_header->source,
        .tag = rx_header->tag,
        .tag_owner = rx_header->tag_owner
    };

    if (rx_header->som)
    {

        if (rx_header->eom)
        {
            mctp_pktq_t rx_queue = {};
            mctp_pktq_enqueue(&rx_queue, mctp_pkt_clone(packet));

            mctp_msgq_update(bus, &rx_queue, &message_ctx);
            mctp_pktq_clear(&rx_queue);
        }
        else
        {
            if (packet->len != MCTP_PKT_MAX_SIZE)
            {
                return mctp_drop_rx_queue(bus, &message_ctx);
            }

            mctp_pktq_t rx_queue = mctp_get_rx_queue(bus, &message_ctx);
            mctp_pktq_clear(&rx_queue);

            mctp_pktq_enqueue(&rx_queue, mctp_pkt_clone(packet));
            mctp_push_rx_queue(bus, &rx_queue, &message_ctx);
        }
    }
    else
    {
        mctp_pktq_t rx_queue = mctp_get_rx_queue(bus, &message_ctx);

        if (mctp_pktq_empty(&rx_queue))
        {
            return;
        }

        if (packet->len != MCTP_PKT_MAX_SIZE && !rx_header->eom)
        {
            return mctp_drop_rx_queue(bus, &message_ctx);
        }

        const mctp_packet_t *front_pkt = mctp_pktq_node_data(
            mctp_pktq_front(&rx_queue)
        );
        const uint8_t expected_pkt_seq = (front_pkt->io.header.pkt_seq + 1) % 4;

        if (rx_header->pkt_seq != expected_pkt_seq)
        {
            return mctp_drop_rx_queue(bus, &message_ctx);
        }

        mctp_pktq_enqueue(&rx_queue, mctp_pkt_clone(packet));

        if (rx_header->eom)
        {
            mctp_msgq_update(bus, &rx_queue, &message_ctx);
            mctp_drop_rx_queue(bus, &message_ctx);
        }
    }
}

static void mctp_msgq_update(
    mctp_bus_t *bus,
    const mctp_pktq_t *rx_queue,
    const mctp_msg_ctx_t *message_ctx
) {
    mctp_message_t message = {
        .context = *message_ctx
    };

    mctp_message_assemble(&message, rx_queue);
    mctp_msgq_enqueue(&bus->rx.msg_queue, message);
}

static mctp_pktq_t mctp_get_rx_queue(
    mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx
) {
    return mctp_pktq_map_node_data(
        mctp_pktq_map_get(
            bus->rx.pktq_map,
            *message_ctx
        )
    );
}

static void mctp_drop_rx_queue(
    mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx
) {
    bus->rx.pktq_map = mctp_pktq_map_remove(
        bus->rx.pktq_map,
        *message_ctx
    );
}

static void mctp_push_rx_queue(
    mctp_bus_t *bus,
    const mctp_pktq_t *rx_queue,
    const mctp_msg_ctx_t *message_ctx    
) {
    bus->rx.pktq_map = mctp_pktq_map_add(
        bus->rx.pktq_map,
        *rx_queue,
        *message_ctx
    );
}
