#include <mctp/core/p_binding.h>
#include <stddef.h>


void mctp_binding_set_bus(
    mctp_binding_t *binding,
    mctp_bus_t *bus
) {
    if (binding == NULL) return;

    binding->bus = bus;
}
