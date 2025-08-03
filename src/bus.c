#include <mctp/core/p_bus.h>
#include <mctp/core/p_binding.h>
#include <mctp/core/base.h>
#include <mctp/util/alloc.h>


mctp_bus_t *mctp_bus_create(
    void
) {
    mctp_bus_t *bus = zalloc(mctp_bus_t);

    return bus;
}

void mctp_bus_destroy(
    mctp_bus_t *bus
) {
    mctp_pktq_map_destroy(bus->rx.pktq_map);
    mctp_msgq_destroy(bus->rx.msgq);
    free(bus);
}

void mctp_bus_set_eid(
    mctp_bus_t *bus,
    const mctp_eid_t eid
) {
    if (bus == NULL) return;

    bus->eid = eid;
}

mctp_eid_t mctp_bus_get_eid(
    mctp_bus_t *bus
) {
    if (bus == NULL)
    {
        return MCTP_EID_NULL;
    }

    return bus->eid;
}

void mctp_bus_transport_bind(
    mctp_bus_t *bus,
    mctp_binding_t *binding
) {
    if (bus == NULL) return;

    if (binding != NULL)
    {
        bus->binding = binding;
        mctp_binding_set_bus(binding, bus);
    }
    else 
    {
        mctp_bus_transport_unbind(bus);
    }
}

void mctp_bus_transport_unbind(
    mctp_bus_t *bus
) {
    if (bus == NULL) return;

    mctp_binding_t *binding = bus->binding;

    mctp_binding_set_bus(binding, NULL);
    bus->binding = NULL;
}
