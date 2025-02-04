#include <mctp/core/bus.h>
#include <stddef.h>
#include <stdlib.h>


mctp_bus_t *mctp_bus_create() 
{
    mctp_bus_t *bus = (mctp_bus_t *)malloc(sizeof(mctp_bus_t));

    return bus;
}

void mctp_bus_destroy(
    mctp_bus_t *bus
) {
    free(bus);
}

void mctp_bus_set_eid(
    mctp_bus_t *bus,
    const mctp_eid_t eid
) {
    bus->eid = eid;
}
