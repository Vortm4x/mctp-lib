#ifndef _MCTP_CORE_BUS_P_H_
#define _MCTP_CORE_BUS_P_H_

#include <mctp/core/bus.h>
#include <mctp/core/p_pktq_map.h>
#include <mctp/core/p_message_queue.h>

typedef struct mctp_bus_t
{
	mctp_eid_t eid;
    mctp_binding_t *binding;
    struct
    {
        mctp_pktq_map_t *pktq_map;
        mctp_msgq_t *msgq;
    }
    rx; 
}
mctp_bus_t;

#endif // _MCTP_CORE_BUS_P_H_
