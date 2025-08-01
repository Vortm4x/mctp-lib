#include <mctp/core/p_mctp.h>
#include <mctp/core/p_bus.h>
#include <mctp/core/p_binding.h>


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
