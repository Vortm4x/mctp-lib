#ifndef _MCTP_CORE_BUS_P_H_
#define _MCTP_CORE_BUS_P_H_

#include <mctp/core/bus.h>

typedef struct mctp_bus_t
{
	mctp_eid_t eid;
    mctp_binding_t *binding;
}
mctp_bus_t;

#endif // _MCTP_CORE_BUS_P_H_
