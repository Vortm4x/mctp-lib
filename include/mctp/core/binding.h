#ifndef _MCTP_CORE_BINDING_H_
#define _MCTP_CORE_BINDING_H_

#include <mctp/core/medium_info.h>
#include <mctp/core/packet.h>


struct mctp_binding_t;
struct mctp_bus_t;

typedef void (*mctp_binding_packet_tx_t)(
    const struct mctp_binding_t *binding,
	const mctp_packet_t *packet
);

typedef struct mctp_binding_t
{
    mctp_binding_packet_tx_t packet_tx;
    mctp_binding_type_t type;
    struct mctp_bus_t *bus;
}
mctp_binding_t;


#endif // _MCTP_CORE_BINDING_H_