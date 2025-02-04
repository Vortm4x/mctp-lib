#ifndef _MCTP_CORE_BUS_H_
#define _MCTP_CORE_BUS_H_

#include <mctp/core/base.h>


typedef struct mctp_bus_t 
{
	mctp_eid_t eid;
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

#endif // _MCTP_CORE_BUS_H_