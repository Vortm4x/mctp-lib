#include <mctp/core/bus.h>
#include <mctp/core/binding.h>
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
    if (bus == NULL) return;

    free(bus);
}

void mctp_bus_set_eid(
    mctp_bus_t *bus,
    const mctp_eid_t eid
) {
    if (bus == NULL) return;

    bus->eid = eid;
}

void mctp_bus_transport_bind(
    mctp_bus_t *bus,
    mctp_binding_t *binding
) {
    if (bus == NULL) return;
    if (binding == NULL) return;

    if (bus->binding != NULL) return;
    if (binding->bus != NULL) return;

    binding->bus = bus;
    bus->binding = binding;
}

void mctp_bus_transport_unbind(
    mctp_bus_t *bus
) {
    if (bus == NULL) return;
    bus->binding = NULL;

    mctp_binding_t *binding = bus->binding;

    if (binding == NULL) return; 
    binding->bus = NULL;
}
