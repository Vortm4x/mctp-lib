#ifndef _MCTP_CORE_BINDING_H_
#define _MCTP_CORE_BINDING_H_

#include <mctp/util/forward_decl.h>
#include <mctp/core/medium_info.h>

forward_decl(mctp_binding_t);
forward_decl(mctp_bus_t);
forward_decl(mctp_packet_t);


typedef void (*mctp_binding_pkt_tx_t)(
    const struct mctp_binding_t *binding,
	const mctp_packet_t *packet
);

typedef struct mctp_binding_t
{
    mctp_binding_pkt_tx_t pkt_tx;
    mctp_binding_type_t type;
    mctp_bus_t *bus;
}
mctp_binding_t;


#endif // _MCTP_CORE_BINDING_H_
