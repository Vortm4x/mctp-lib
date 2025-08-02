#ifndef _MCTP_CORE_MCTP_H_
#define _MCTP_CORE_MCTP_H_

#include <mctp/util/forward_decl.h>
#include <mctp/util/extern_c.h>
#include <stdbool.h>

forward_decl(mctp_bus_t);
forward_decl(mctp_pkt_t);
forward_decl(mctp_pktq_t);


EXTERN_C_BEGIN

void mctp_pktq_tx(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const bool drain
);

void mctp_packet_tx(
    const mctp_bus_t *bus,
    const mctp_pkt_t *packet
);

EXTERN_C_END

#endif // _MCTP_CORE_MCTP_H_
