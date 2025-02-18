#ifndef _MCTP_CORE_BUS_H_
#define _MCTP_CORE_BUS_H_

#include <mctp/core/base.h>
#include <mctp/core/binding.h>
#include <mctp/core/packet_queue.h>
#include <mctp/core/message_queue.h>


typedef struct mctp_bus_t
{
	mctp_eid_t eid;
    mctp_binding_t *binding;
    struct
    {
        mctp_pktq_t packet_queue;
        mctp_msgq_t message_queue;
    }
    rx;
}
mctp_bus_t;


mctp_bus_t *mctp_bus_create();

void mctp_bus_destroy(
    mctp_bus_t *bus
);

void mctp_bus_set_eid(
    mctp_bus_t *bus,
    const mctp_eid_t eid
);

void mctp_bus_transport_bind(
    mctp_bus_t *bus,
    mctp_binding_t *binding
);


#endif // _MCTP_CORE_BUS_H_
