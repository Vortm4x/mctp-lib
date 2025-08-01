#ifndef _MCTP_CORE_BUS_H_
#define _MCTP_CORE_BUS_H_

#include <mctp/core/base.h>
#include <mctp/util/extern_c.h>
#include <mctp/util/forward_decl.h>

forward_decl(mctp_binding_t);
forward_decl(mctp_bus_t);


EXTERN_C_BEGIN

mctp_bus_t *mctp_bus_create(
    void
);

void mctp_bus_destroy(
    mctp_bus_t *bus
);

void mctp_bus_set_eid(
    mctp_bus_t *bus,
    const mctp_eid_t eid
);

mctp_eid_t mctp_bus_get_eid(
    mctp_bus_t *bus
);

void mctp_bus_transport_bind(
    mctp_bus_t *bus,
    mctp_binding_t *binding
);

void mctp_bus_transport_unbind(
    mctp_bus_t *bus
);

EXTERN_C_END

#endif // _MCTP_CORE_BUS_H_
