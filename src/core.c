#include <mctp/core/mctp.h>
#include <mctp/core/p_bus.h>
#include <mctp/core/p_binding.h>
#include <mctp/core/p_packet.h>
#include <mctp/core/p_message.h>
#include <mctp/core/p_message_queue.h>
#include <mctp/core/packet_queue.h>


static mctp_pktq_t *mctp_get_rx_queue(
    mctp_bus_t *bus,
    const mctp_msg_ctx_raw_t raw_ctx
);

static void mctp_drop_rx_queue(
    mctp_bus_t *bus,
    const mctp_msg_ctx_raw_t raw_ctx
);

static void mctp_push_rx_queue(
    mctp_bus_t *bus,
    mctp_pktq_t *rx_queue,
    const mctp_msg_ctx_raw_t raw_ctx
);

static void mctp_msgq_update(
    mctp_bus_t *bus,
    const mctp_pktq_t *rx_queue,
    const mctp_msg_ctx_t *msg_ctx
);


void mctp_pktq_tx(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const bool drain
) {
    mctp_pktq_node_t *node = mctp_pktq_front(tx_queue);

    while (node != NULL)
    {
        mctp_pkt_t *packet = mctp_pktq_node_data(node);

        mctp_packet_tx(bus, packet);
        node = mctp_pktq_node_next(node);

        if (drain) {
            mctp_pktq_dequeue(tx_queue);
        }
    }
}

void mctp_packet_tx(
    const mctp_bus_t *bus,
    const mctp_pkt_t *packet
) {
    if (bus == NULL) return;
    if (packet == NULL) return;

    const mctp_binding_t *binding = bus->binding;

    if (binding == NULL) return;
    if (binding->packet_tx == NULL) return;

    binding->packet_tx(binding, packet);
}

void mctp_packet_rx(
    mctp_bus_t *bus,
    const mctp_pkt_t *packet
) {
    const mctp_pkt_hdr_t *rx_header = &packet->io.header;

    if (rx_header->version  != MCTP_PKT_HDR_VER
     || rx_header->dest     != bus->eid)
    {
        return;
    }

    const mctp_msg_ctx_t ctx = {
        .remote_eid = rx_header->source,
        .tag        = rx_header->tag,
        .tag_owner  = rx_header->tag_owner
    };
    const mctp_msg_ctx_raw_t raw_ctx = MCTP_MSG_CTX_RAW(ctx);

    if (rx_header->som)
    {
        if (rx_header->eom)
        {
            mctp_pktq_t *rx_queue = mctp_pktq_create();
            mctp_pktq_enqueue(rx_queue, mctp_pkt_clone(packet));

            mctp_msgq_update(bus, rx_queue, &ctx);
            mctp_pktq_destroy(rx_queue);
        }
        else
        {
            if (packet->len != MCTP_PKT_MAX_SIZE)
            {
                mctp_drop_rx_queue(bus, raw_ctx);
                return;
            }

            mctp_pktq_t *rx_queue = mctp_get_rx_queue(bus, raw_ctx);
    
            if (rx_queue == NULL)
            {
                rx_queue = mctp_pktq_create();
            }

            mctp_pktq_clear(rx_queue);
            mctp_pktq_enqueue(rx_queue, mctp_pkt_clone(packet));
            mctp_push_rx_queue(bus, rx_queue, raw_ctx);
        }
    }
    else
    {
        mctp_pktq_t *rx_queue = mctp_get_rx_queue(bus, raw_ctx);

        if (mctp_pktq_empty(rx_queue))
        {
            return;
        }

        if (packet->len != MCTP_PKT_MAX_SIZE && !rx_header->eom)
        {
            mctp_drop_rx_queue(bus, raw_ctx);
            return;
        }

        const mctp_pkt_t *front_pkt = mctp_pktq_node_data(
            mctp_pktq_front(rx_queue)
        );
        const uint8_t expected_seq_num = (uint8_t)(front_pkt->io.header.seq_num + 1) % 4;

        if (rx_header->seq_num != expected_seq_num)
        {
            mctp_drop_rx_queue(bus, raw_ctx);
            return;
        }

        mctp_pktq_enqueue(rx_queue, mctp_pkt_clone(packet));

        if (rx_header->eom)
        {
            mctp_msgq_update(bus, rx_queue, &ctx);
            mctp_drop_rx_queue(bus, raw_ctx);
            return;
        }
    }
}

static mctp_pktq_t *mctp_get_rx_queue(
    mctp_bus_t *bus,
    const mctp_msg_ctx_raw_t raw_ctx
) {
    return mctp_pktq_map_node_data(
        mctp_pktq_map_get(
            bus->rx.pktq_map,
            raw_ctx
        )
    );
}

static void mctp_drop_rx_queue(
    mctp_bus_t *bus,
    const mctp_msg_ctx_raw_t raw_ctx
) {
    bus->rx.pktq_map = mctp_pktq_map_remove(
        bus->rx.pktq_map,
        raw_ctx
    );
}

static void mctp_push_rx_queue(
    mctp_bus_t *bus,
    mctp_pktq_t *rx_queue,
    const mctp_msg_ctx_raw_t raw_ctx
) {
    bus->rx.pktq_map = mctp_pktq_map_add(
        bus->rx.pktq_map,
        rx_queue,
        raw_ctx
    );
}

static void mctp_msgq_update(
    mctp_bus_t *bus,
    const mctp_pktq_t *rx_queue,
    const mctp_msg_ctx_t *msg_ctx
) {
    mctp_msg_t message = {
        .ctx = *msg_ctx
    };

    mctp_message_assemble(rx_queue, &message.data, &message.len);
    mctp_msgq_enqueue(bus->rx.msgq, message);
}
