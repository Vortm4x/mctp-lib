#ifndef _MCTP_CORE_BINDING_H_
#define _MCTP_CORE_BINDING_H_

#include <mctp/core/medium_info.h>
#include <mctp/core/packet_queue.h>
#include <stdint.h>
#include <stdlib.h>



typedef struct
{
    mctp_binding_type_t type;
    mctp_pktq_t tx_queue;
}
mctp_binding_t;



#endif // _MCTP_CORE_BINDING_H_