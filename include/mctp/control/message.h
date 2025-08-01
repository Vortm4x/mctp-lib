#ifndef _MCTP_CONTROL_MESSAGE_H_
#define _MCTP_CONTROL_MESSAGE_H_

#include <mctp/control/request.h>
#include <mctp/control/response.h>
#include <mctp/core/base.h>
#include <mctp/core/mctp.h>
#include <mctp/util/extern_c.h>


typedef enum __attribute__ ((__packed__))
{
    MCTP_CTRL_CMD_RESERVED                      = 0x00,
    MCTP_CTRL_CMD_SET_ENDPOINT_ID               = 0x01,
    MCTP_CTRL_CMD_GET_ENDPOINT_ID               = 0x02,
    MCTP_CTRL_CMD_GET_ENDPOINT_UUID             = 0x03,
    MCTP_CTRL_CMD_GET_VERSION_SUPPORT           = 0x04,
    MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT      = 0x05,
    MCTP_CTRL_CMD_GET_VENDOR_MESSAGE_SUPPORT    = 0x06,
    MCTP_CTRL_CMD_RESOLVE_ENDPOINT_ID           = 0x07,
    MCTP_CTRL_CMD_ALLOCATE_ENDPOINT_IDS         = 0x08,
    MCTP_CTRL_CMD_ROUTING_INFO_UPDATE           = 0x09,
    MCTP_CTRL_CMD_GET_ROUTING_TABLE_ENTRIES     = 0x0A,
    MCTP_CTRL_CMD_PREP_ENDPOINT_DISCOVERY       = 0x0B,
    MCTP_CTRL_CMD_ENDPOINT_DISCOVERY            = 0x0C,
    MCTP_CTRL_CMD_DISCOVERY_NOTIFY              = 0x0D,
    MCTP_CTRL_CMD_GET_NETWORK_ID                = 0x0E,
    MCTP_CTRL_CMD_QUERY_HOP                     = 0x0F,
    MCTP_CTRL_CMD_RESOLVE_UUID                  = 0x10,
    MCTP_CTRL_CMD_QUERY_RATE_LIMIT              = 0x11,
    MCTP_CTRL_CMD_REQUEST_TX_RATE_LIMIT         = 0x12,
    MCTP_CTRL_CMD_UPDATE_RATE_LIMIT             = 0x13,
    MCTP_CTRL_CMD_QUERY_SUPP_IF                 = 0x14,
    MCTP_CTRL_CMD_MAX                           = 0x15,
}
mctp_ctrl_cmd_t;

typedef enum __attribute__ ((__packed__))
{
    MCTP_CTRL_CC_SUCCESS                    = 0x00,
    MCTP_CTRL_CC_ERROR_GENERIC              = 0x01,
    MCTP_CTRL_CC_ERROR_INVALID_DATA         = 0x02,
    MCTP_CTRL_CC_ERROR_INVALID_LENGTH       = 0x03,
    MCTP_CTRL_CC_ERROR_NOT_READY            = 0x04,
    MCTP_CTRL_CC_ERROR_UNSUPPORTED_CMD      = 0x05,
    MCTP_CTRL_CC_MSG_TYPE_NOT_SUPPORTED     = 0x80,
}
mctp_ctrl_cc_t;


typedef struct __attribute__ ((__packed__))
{
    mctp_generic_header_t base;
    uint8_t instance    : 5;
    uint8_t             : 1;
    bool datagram       : 1;
    bool request        : 1;
    mctp_ctrl_cmd_t command;
}
mctp_ctrl_header_t;

#define MCTP_CTRL_HDR_SIZE (sizeof(mctp_ctrl_header_t))


EXTERN_C_BEGIN

void mctp_ctrl_request_prepare(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const mctp_eid_t dest,
    const mctp_ctrl_cmd_t command,
    const bool datagram,
    const uint8_t payload_data[],
    const size_t payload_len
);

void mctp_ctrl_message_prepare(
    mctp_pktq_t *tx_queue,
    const mctp_bus_t *bus,
    const mctp_msg_ctx_t *message_ctx,
    const mctp_ctrl_header_t *header,
    const uint8_t payload_data[],
    const size_t payload_len
);

EXTERN_C_END

#endif // _MCTP_CONTROL_MESSAGE_H_
