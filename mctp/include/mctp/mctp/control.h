#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <mctp/base.h>
#include <mctp/host.h>

#define MCTP_MSG_TYPE_BASE_SPEC 0xFF


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
    MCTP_CTRL_CMD_QUERY_SUPP_IF    = 0x14,
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

typedef enum __attribute__ ((__packed__))
{
    MCTP_EID_ALLOC_OP_ALLOC           = 0b00,
    MCTP_EID_ALLOC_OP_FORCE           = 0b01,
    MCTP_EID_ALLOC_OP_INFO            = 0b10,
    MCTP_EID_ALLOC_OP_RESERVED_b11    = 0b11,
}
mctp_eid_alloc_op_t;

typedef enum __attribute__ ((__packed__))
{
    MCTP_ROUTING_TABLE_ENTRY_TYPE_SINGLE_NON_BRIDGE    = 0b00,
    MCTP_ROUTING_TABLE_ENTRY_TYPE_RANGE_BRIDGE_INCLUDE = 0b01,
    MCTP_ROUTING_TABLE_ENTRY_TYPE_SINGLE_BRIDGE        = 0b10,
    MCTP_ROUTING_TABLE_ENTRY_TYPE_RANGE_BRIDGE_EXCLUDE = 0b11,
}
mctp_routing_table_entry_type_t;

typedef union __attribute__ ((__packed__))
{
    struct
    {
        uint8_t alpha;
        uint8_t update;
        uint8_t minor;
        uint8_t major;
    };
    uint32_t version;
}
mctp_ver_t;

typedef struct __attribute__ ((__packed__))
{
    mctp_routing_table_entry_type_t type    : 2;
    uint8_t                                 : 6;
    uint8_t range_size;
    mctp_eid_t start_eid;
    uint8_t physical_address[];
}
mctp_routing_info_upd_t;

typedef struct __attribute__ ((__packed__))
{
    uint8_t eid_range_size;
    mctp_eid_t start_eid;
    uint8_t port                            : 5;
    bool is_static                          : 1;
    mctp_routing_table_entry_type_t type    : 2;
    mctp_binding_type_t binding_type;
    mctp_physical_medium_t medium;
    uint8_t physical_address_size;
    uint8_t physical_address[];
}
mctp_routing_table_entry_t;

typedef struct __attribute__ ((__packed__))
{
    mctp_eid_t eid;      
    mctp_binding_type_t binding_type;       
    mctp_physical_medium_t medium;         
    uint8_t physical_address_size;          
    uint8_t physical_address[];
}
mctp_resolve_uuid_entry_t;

typedef struct __attribute__ ((__packed__))
{
    mctp_hif_type_t hif_type;
    mctp_eid_t eid;
}
mctp_interface_t;


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


__MCTP_RESP_LAYOUT()
mctp_resp_error_t;


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


// MCTP_CTRL_CMD_GET_VERSION_SUPPORT
__MCTP_REQ_LAYOUT(
    mctp_msg_type_t msg_type;
)
mctp_req_get_mctp_ver_t;

__MCTP_RESP_LAYOUT(
    uint8_t version_count;
    mctp_ver_t version[];
)
mctp_resp_get_mctp_ver_t;


// MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT
__MCTP_REQ_LAYOUT()
mctp_req_get_msg_type_t;

__MCTP_RESP_LAYOUT(
    uint8_t msg_type_count;
    mctp_msg_type_t msg_types[];
)
mctp_resp_get_msg_type_t;


// MCTP_CTRL_CMD_GET_VENDOR_MESSAGE_SUPPORT
__MCTP_REQ_LAYOUT(
    uint8_t vendor_id_selector;
)
mctp_req_get_vendor_t;

__MCTP_RESP_LAYOUT(
    uint8_t vendor_id_selector;
)
mctp_resp_get_vendor_t;


// MCTP_CTRL_CMD_RESOLVE_ENDPOINT_ID
__MCTP_REQ_LAYOUT(
    mctp_eid_t target_eid;
)
mctp_req_resolve_eid_t;

__MCTP_RESP_LAYOUT(

    mctp_eid_t bridge_eid;
    uint8_t physical_address[];
)
mctp_resp_resolve_eid_t;


// MCTP_CTRL_CMD_ALLOCATE_ENDPOINT_IDS
__MCTP_REQ_LAYOUT(
    mctp_eid_alloc_op_t operation   : 2;
    uint8_t                         : 6;
    uint8_t eid_pool_size;
    mctp_eid_t start_eid;
)
mctp_req_alloc_eid_t;

__MCTP_RESP_LAYOUT(
    mctp_eid_assign_status_t eid_assign_status  : 2;
    uint8_t                                     : 6;
    uint8_t eid_pool_size;
    mctp_eid_t start_eid;
)
mctp_resp_alloc_eid_t;


// MCTP_CTRL_CMD_ROUTING_INFO_UPDATE
__MCTP_REQ_LAYOUT(
    uint8_t routing_info_upd_count;
    mctp_routing_info_upd_t routing_info_upd;
)
mctp_req_routing_info_upd_t;

__MCTP_RESP_LAYOUT()
mctp_resp_routing_info_upd_t;


// MCTP_CTRL_CMD_GET_ROUTING_TABLE_ENTRIES
__MCTP_REQ_LAYOUT(
    uint8_t entry_handle : 8;
)
mctp_req_get_routing_table_t;

__MCTP_RESP_LAYOUT(
    uint8_t next_entry_handle;
    uint8_t routing_table_entry_count;
    mctp_routing_table_entry_t routing_table_entry; 
)
mctp_resp_get_routing_table_t;


// MCTP_CTRL_CMD_PREP_ENDPOINT_DISCOVERY
__MCTP_REQ_LAYOUT()
mctp_req_prep_endpoint_discovery_t;

__MCTP_RESP_LAYOUT()
mctp_resp_prep_endpoint_discovery_t;


// MCTP_CTRL_CMD_ENDPOINT_DISCOVERY
__MCTP_REQ_LAYOUT()
mctp_req_endpoint_discovery_t;

__MCTP_RESP_LAYOUT()
mctp_resp_endpoint_discovery_t;


// MCTP_CTRL_CMD_DISCOVERY_NOTIFY
__MCTP_REQ_LAYOUT()
mctp_req_discovery_notify_t;

__MCTP_RESP_LAYOUT()
mctp_resp_discovery_notify_t;


// MCTP_CTRL_CMD_GET_NETWORK_ID
__MCTP_REQ_LAYOUT()
mctp_req_get_network_id_t;

__MCTP_RESP_LAYOUT(
    uint8_t network_id[16];
)
mctp_resp_get_network_id_t;


// MCTP_CTRL_CMD_QUERY_HOP
__MCTP_REQ_LAYOUT(
    mctp_eid_t target_eid;
    mctp_msg_type_t msg_type;
)
mctp_req_query_hop_t;

__MCTP_RESP_LAYOUT(
    mctp_eid_t next_bridge;
    mctp_msg_type_t msg_type;
    uint16_t maxt_tu_in;
    uint16_t maxt_tu_out;
)
mctp_resp_query_hop_t;



// MCTP_CTRL_CMD_RESOLVE_UUID
__MCTP_REQ_LAYOUT(
    uint8_t uuid[16];
    uint8_t target_eid;
)
mctp_req_resolve_uuid_t;

__MCTP_RESP_LAYOUT(
    uint8_t next_entry_handle;
    uint8_t entries_count;
    mctp_resolve_uuid_entry_t first_entry;
)
mctp_resp_resolve_uuid_t;


// MCTP_CTRL_CMD_QUERY_RATE_LIMIT
__MCTP_REQ_LAYOUT()
mctp_req_query_rate_limit_t;

__MCTP_RESP_LAYOUT(
    uint32_t rx_buffer_size;
    uint32_t rx_rate;
    uint32_t tx_max_rate_sup;
    uint32_t tx_min_rate_sup;
    uint32_t max_burst_sup      : 24;
    uint32_t eid_tx_max_burst   : 24;
    uint32_t eid_tx_max_rate;
    bool tx_limiting_resp_only  : 1;
    bool tx_limiting_sup        : 1;
    uint8_t                     : 6;
)
mctp_resp_query_rate_limit_t;


// MCTP_CTRL_CMD_REQUEST_TX_RATE_LIMIT
__MCTP_REQ_LAYOUT(
    uint32_t eid_tx_max_burst : 24;
    uint32_t eid_tx_max_rate;
)
mctp_req_request_tx_rate_limit_t;

__MCTP_RESP_LAYOUT(
    uint32_t eid_tx_burst : 24;
    uint32_t eid_tx_rate;
)
mctp_resp_request_tx_rate_limit_t;


// MCTP_CTRL_CMD_UPDATE_RATE_LIMIT
__MCTP_REQ_LAYOUT(
    uint32_t eid_tx_burst : 24;
    uint32_t eid_tx_rate;
)
mctp_req_upd_rate_limit_t;

__MCTP_RESP_LAYOUT()
mctp_resp_upd_rate_limit_t;


// MCTP_CTRL_CMD_QUERY_SUPP_IF
__MCTP_REQ_LAYOUT()
mctp_req_query_supp_if_t;

__MCTP_RESP_LAYOUT(
    uint8_t interface_count;
    mctp_interface_t firts_intefrace;
)
mctp_resp_query_supp_if_t;


#endif // CONTROL_H