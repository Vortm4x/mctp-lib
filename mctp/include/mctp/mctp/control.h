#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <mctp/base.h>


typedef enum __attribute__ ((__packed__))
{
    MCTP_CTRL_CMD_RESERVED                          = 0x00,
    MCTP_CTRL_CMD_SET_ENDPOINT_ID                   = 0x01,
    MCTP_CTRL_CMD_GET_ENDPOINT_ID                   = 0x02,
    MCTP_CTRL_CMD_GET_ENDPOINT_UUID                 = 0x03,
    MCTP_CTRL_CMD_GET_VERSION_SUPPORT               = 0x04,
    MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT          = 0x05,
    MCTP_CTRL_CMD_GET_VENDOR_MESSAGE_SUPPORT        = 0x06,
    MCTP_CTRL_CMD_RESOLVE_ENDPOINT_ID               = 0x07,
    MCTP_CTRL_CMD_ALLOCATE_ENDPOINT_IDS             = 0x08,
    MCTP_CTRL_CMD_ROUTING_INFO_UPDATE               = 0x09,
    MCTP_CTRL_CMD_GET_ROUTING_TABLE_ENTRIES         = 0x0A,
    MCTP_CTRL_CMD_PREPARE_FOR_ENDPOINT_DISCOVERY    = 0x0B,
    MCTP_CTRL_CMD_ENDPOINT_DISCOVERY                = 0x0C,
    MCTP_CTRL_CMD_DISCOVERY_NOTIFY                  = 0x0D,
    MCTP_CTRL_CMD_GET_NETWORK_ID                    = 0x0E,
    MCTP_CTRL_CMD_QUERY_HOP                         = 0x0F,
    MCTP_CTRL_CMD_RESOLVE_UUID                      = 0x10,
    MCTP_CTRL_CMD_QUERY_RATE_LIMIT                  = 0x11,
    MCTP_CTRL_CMD_REQUEST_TX_RATE_LIMIT             = 0x12,
    MCTP_CTRL_CMD_UPDATE_RATE_LIMIT                 = 0x13,
    MCTP_CTRL_CMD_QUERY_SUPPORTED_INTERFACES        = 0x14,
    MCTP_CTRL_CMD_MAX                               = 0x15,    
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


typedef struct __attribute__ ((__packed__))
{
    mctp_ctrl_header_t header;
    mctp_ctrl_cc_t completion_code;
}
mctp_resp_error_t;


// Message fields types
typedef enum __attribute__ ((__packed__))
{
    MCTP_SET_EID_OP_SET_EID     = 0b00,
    MCTP_SET_EID_OP_FORCE_EID   = 0b01,
    MCTP_SET_EID_OP_RESET_EID   = 0b10,
    MCTP_SET_EID_OP_DISCOVERED  = 0b11,
}
mctp_set_eid_op_t;

typedef enum __attribute__ ((__packed__))
{
    MCTP_EID_ALLOC_STATUS_POOL_NOT_USED = 0b00,
    MCTP_EID_ALLOC_STATUS_POOL_REQUIRED = 0b01,
    MCTP_EID_ALLOC_STATUS_POOL_EXIST    = 0b10,
    MCTP_EID_ALLOC_STATUS_RESERVED_b11  = 0b11,
}
mctp_eid_alloc_status_t;

typedef enum __attribute__ ((__packed__))
{
    MCTP_EID_ASSIGN_STATUS_ACCEPTED        = 0b00,
    MCTP_EID_ASSIGN_STATUS_REJECTED        = 0b01,
    MCTP_EID_ASSIGN_STATUS_RESERVED_b10    = 0b10,
    MCTP_EID_ASSIGN_STATUS_RESERVED_b11    = 0b11,
}
mctp_eid_assign_status_t;

typedef enum __attribute__ ((__packed__))
{
    MCTP_EID_TYPE_DYNAMIC           = 0b00,
    MCTP_EID_TYPE_STATIC            = 0b01,
    MCTP_EID_TYPE_STATIC_MATCH      = 0b10,
    MCTP_EID_TYPE_STATIC_NO_MATCH   = 0b11,
}
mctp_eid_type_t;

typedef enum __attribute__ ((__packed__))
{
    MCTP_ENDPOINT_TYPE_SIMPLE        = 0b00,
    MCTP_ENDPOINT_TYPE_BRIDGE        = 0b01,
    MCTP_ENDPOINT_TYPE_BUS_OWNER     = 0b01,
    MCTP_ENDPOINT_TYPE_RESERVED_b10  = 0b10,
    MCTP_ENDPOINT_TYPE_RESERVED_b11  = 0b11,
}
mctp_endpoint_type_t;

// MCTP Control messasge layouts
#define __MCTP_REQ_LAYOUT(...)                  \
    typedef struct __attribute__ ((__packed__)) \
    {                                           \
        mctp_ctrl_header_t header;              \
        __VA_ARGS__                             \
    }

#define __MCTP_RESP_LAYOUT(...)                 \
    typedef struct __attribute__ ((__packed__)) \
    {                                           \
        mctp_ctrl_header_t header;              \
        mctp_ctrl_cc_t completion_code;         \
        __VA_ARGS__                             \
    }


// MCTP_CTRL_CMD_SET_ENDPOINT_ID
__MCTP_REQ_LAYOUT(
    mctp_set_eid_op_t operation : 2;
    uint8_t                     : 6;
    mctp_eid_t eid;
)
mctp_req_set_endpoint_id_t;

__MCTP_RESP_LAYOUT(
    mctp_eid_alloc_status_t eid_alloc_status    : 2; 
    uint8_t                                     : 2;
    mctp_eid_assign_status_t eid_assign_status  : 2;
    uint8_t                                     : 2;
    mctp_eid_t eid_setting;
    uint8_t eid_pool_size;
)
mctp_resp_set_endpoint_id_t;


// MCTP_CTRL_CMD_GET_ENDPOINT_ID
__MCTP_REQ_LAYOUT()
mctp_req_get_endpoint_id_t;

__MCTP_RESP_LAYOUT(
    mctp_eid_t eid;
    mctp_eid_type_t eid_type            : 2;
    uint8_t                             : 2;
    mctp_endpoint_type_t endpoint_type  : 2;
    uint8_t                             : 2;
    uint8_t medium_info;
)
mctp_resp_get_endpoint_id_t;


// MCTP_CTRL_CMD_GET_ENDPOINT_UUID

__MCTP_REQ_LAYOUT()
mctp_req_get_endpoint_uuid_t;

__MCTP_RESP_LAYOUT(
    uint8_t uuid[16];
)
mctp_resp_get_endpoint_uuid_t;


#endif // CONTROL_H