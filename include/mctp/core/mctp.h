#ifndef _MCTP_CORE_MCTP_H_
#define _MCTP_CORE_MCTP_H_

#include <mctp/core/bus.h>
#include <mctp/util/extern_c.h>

#define MCTP_PKT_HDR_VER 0b0001


EXTERN_C_BEGIN

uint8_t mctp_get_message_tag();

void mctp_message_disassemble(
    const mctp_bus_t *bus,
    const mctp_message_t *message,
    mctp_pktq_t *tx_queue
);

void mctp_message_assemble(
    mctp_message_t *message,
    const mctp_pktq_t *rx_queue
);

void mctp_pktq_drain(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus
);

void mctp_pktq_tx(
    const mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus
);

void mctp_packet_tx(
    const mctp_bus_t *bus,
    const mctp_packet_t *packet
);

void mctp_packet_rx(
    mctp_bus_t *bus,
    const mctp_packet_t *packet
);

EXTERN_C_END

#endif // _MCTP_CORE_MCTP_H_
