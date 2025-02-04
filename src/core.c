#include <mctp/core/mctp.h>
#include <stdio.h>


uint8_t mctp_get_message_tag()
{
    static uint8_t msg_tag = 0;

    return msg_tag++;
}

void mctp_message_tx(
    const mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx,
    const uint8_t message_data[],
    const size_t message_len
) {
    mctp_transport_header_t header = {
        .version = MCTP_PKT_HDR_VER,
        .dest = message_ctx->eid,
        .source = bus->eid,
        .tag = message_ctx->message_tag,
        .tag_owner = message_ctx->tag_owner,
    };
}
