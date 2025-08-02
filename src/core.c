#include <mctp/core/p_mctp.h>
#include <mctp/core/p_bus.h>
#include <mctp/core/p_binding.h>
#include <mctp/core/packet_queue.h>


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
