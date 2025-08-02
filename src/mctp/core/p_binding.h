#ifndef _MCTP_CORE_BINDING_P_H_
#define _MCTP_CORE_BINDING_P_H_

#include <mctp/core/medium_info.h>
#include <mctp/util/forward_decl.h>
#include <mctp/util/extern_c.h>

forward_decl(mctp_binding_t);
forward_decl(mctp_bus_t);
forward_decl(mctp_pkt_t);


typedef void (*mctp_binding_pkt_tx_t)(
    const mctp_binding_t *binding,
	const mctp_pkt_t *packet
);


typedef struct mctp_binding_t
{
    mctp_binding_pkt_tx_t packet_tx;
    mctp_binding_type_t type;
    mctp_bus_t *bus;
}
mctp_binding_t;


EXTERN_C_BEGIN

void mctp_binding_set_bus(
    mctp_binding_t *binding,
    mctp_bus_t *bus
);

EXTERN_C_END

#endif // _MCTP_CORE_BINDING_P_H_
