#ifndef _MCTP_CORE_MCTP_H_
#define _MCTP_CORE_MCTP_H_

#include <mctp/util/forward_decl.h>
#include <mctp/util/extern_c.h>

forward_decl(mctp_bus_t);
forward_decl(mctp_pkt_t);


EXTERN_C_BEGIN

void mctp_packet_tx(
    const mctp_bus_t *bus,
    const mctp_pkt_t *packet
);

EXTERN_C_END

#endif // _MCTP_CORE_MCTP_H_
