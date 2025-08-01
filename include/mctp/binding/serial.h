#ifndef _MCTP_BINDING_SERIAL_H_
#define _MCTP_BINDING_SERIAL_H_

#include <mctp/util/forward_decl.h>
#include <mctp/util/extern_c.h>
#include <stdint.h>

forward_decl(mctp_serial_t);
forward_decl(mctp_binding_t);


typedef void (*mctp_serial_byte_tx_t)(
    const uint8_t byte
);


EXTERN_C_BEGIN

mctp_serial_t* mctp_serial_create(
    void
);

void mctp_serial_destroy(
    mctp_serial_t *serial
);

void mctp_serial_set_byte_tx(
    mctp_serial_t *serial,
    mctp_serial_byte_tx_t byte_tx
);

mctp_binding_t *mctp_serial_get_binding(
    mctp_serial_t *serial
);

void mctp_serial_byte_rx(
    const mctp_binding_t *binding,
	const uint8_t byte
);

EXTERN_C_END

#endif // _MCTP_BINDING_SERIAL_H_
