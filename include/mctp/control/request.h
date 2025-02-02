#ifndef _MCTP_CONTROL_REQUEST_H_
#define _MCTP_CONTROL_REQUEST_H_

#include <mctp/control/constants.h>
#include <mctp/control/types.h>


#define MCTP_CTRL_REQ_LAYOUT(...)               \
    typedef struct __attribute__ ((__packed__)) \
    {                                           \
        struct __attribute__ ((__packed__))     \
        {                                       \
            int MCTB_ZERO_PAYLOAD_STUB[0];      \
        };                                      \
        __VA_ARGS__                             \
    }


// MCTP_CTRL_CMD_SET_ENDPOINT_ID
MCTP_CTRL_REQ_LAYOUT(
    mctp_set_eid_op_t operation : 2;
    uint8_t                     : 6;
    mctp_eid_t eid;
)
mctp_req_set_endpoint_id_t;


// MCTP_CTRL_CMD_GET_ENDPOINT_ID
MCTP_CTRL_REQ_LAYOUT()
mctp_req_get_endpoint_id_t;


// MCTP_CTRL_CMD_GET_ENDPOINT_UUID
MCTP_CTRL_REQ_LAYOUT()
mctp_req_get_endpoint_uuid_t;


// MCTP_CTRL_CMD_GET_VERSION_SUPPORT
MCTP_CTRL_REQ_LAYOUT(
    mctp_msg_type_t msg_type;
)
mctp_req_get_mctp_ver_t;


// MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT
MCTP_CTRL_REQ_LAYOUT()
mctp_req_get_msg_type_t;


// MCTP_CTRL_CMD_GET_VENDOR_MESSAGE_SUPPORT
MCTP_CTRL_REQ_LAYOUT(
    uint8_t vendor_id_selector;
)
mctp_req_get_vendor_t;


// MCTP_CTRL_CMD_RESOLVE_ENDPOINT_ID
MCTP_CTRL_REQ_LAYOUT(
    mctp_eid_t target_eid;
)
mctp_req_resolve_eid_t;


// MCTP_CTRL_CMD_ALLOCATE_ENDPOINT_IDS
MCTP_CTRL_REQ_LAYOUT(
    mctp_eid_alloc_op_t operation   : 2;
    uint8_t                         : 6;
    uint8_t eid_pool_size;
    mctp_eid_t start_eid;
)
mctp_req_alloc_eid_t;


// MCTP_CTRL_CMD_ROUTING_INFO_UPDATE
MCTP_CTRL_REQ_LAYOUT(
    uint8_t routing_info_upd_count;
    mctp_routing_info_upd_t routing_info_upd;
)
mctp_req_routing_info_upd_t;

// MCTP_CTRL_CMD_GET_ROUTING_TABLE_ENTRIES
MCTP_CTRL_REQ_LAYOUT(
    uint8_t entry_handle : 8;
)
mctp_req_get_routing_table_t;


// MCTP_CTRL_CMD_PREP_ENDPOINT_DISCOVERY
MCTP_CTRL_REQ_LAYOUT()
mctp_req_prep_endpoint_discovery_t;


// MCTP_CTRL_CMD_ENDPOINT_DISCOVERY
MCTP_CTRL_REQ_LAYOUT()
mctp_req_endpoint_discovery_t;


// MCTP_CTRL_CMD_DISCOVERY_NOTIFY
MCTP_CTRL_REQ_LAYOUT()
mctp_req_discovery_notify_t;


// MCTP_CTRL_CMD_GET_NETWORK_ID
MCTP_CTRL_REQ_LAYOUT()
mctp_req_get_network_id_t;


// MCTP_CTRL_CMD_QUERY_HOP
MCTP_CTRL_REQ_LAYOUT(
    mctp_eid_t target_eid;
    mctp_msg_type_t msg_type;
)
mctp_req_query_hop_t;


// MCTP_CTRL_CMD_RESOLVE_UUID
MCTP_CTRL_REQ_LAYOUT(
    uint8_t uuid[16];
    uint8_t target_eid;
)
mctp_req_resolve_uuid_t;


// MCTP_CTRL_CMD_QUERY_RATE_LIMIT
MCTP_CTRL_REQ_LAYOUT()
mctp_req_query_rate_limit_t;


// MCTP_CTRL_CMD_REQUEST_TX_RATE_LIMIT
MCTP_CTRL_REQ_LAYOUT(
    uint32_t eid_tx_max_burst : 24;
    uint32_t eid_tx_max_rate;
)
mctp_req_request_tx_rate_limit_t;


// MCTP_CTRL_CMD_UPDATE_RATE_LIMIT
MCTP_CTRL_REQ_LAYOUT(
    uint32_t eid_tx_burst : 24;
    uint32_t eid_tx_rate;
)
mctp_req_upd_rate_limit_t;


// MCTP_CTRL_CMD_QUERY_SUPP_IF
MCTP_CTRL_REQ_LAYOUT()
mctp_req_query_supp_if_t;


#define MCTP_CTRL_TRANSP_REQ_LAYOUT(...)    \
    MCTP_CTRL_REQ_LAYOUT(                   \
        mctp_binding_type_t binding_type;   \
        mctp_physical_medium_t medium;      \
        __VA_ARGS__                         \
    )


#define mctp_ctrl_get_command_by_request(request) _Generic((request), \
    mctp_req_set_endpoint_id_t:         MCTP_CTRL_CMD_SET_ENDPOINT_ID, \
    mctp_req_get_endpoint_id_t:         MCTP_CTRL_CMD_GET_ENDPOINT_ID, \
    mctp_req_get_endpoint_uuid_t:       MCTP_CTRL_CMD_GET_ENDPOINT_UUID, \
    mctp_req_get_mctp_ver_t:            MCTP_CTRL_CMD_GET_VERSION_SUPPORT, \
    mctp_req_get_msg_type_t:            MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT, \
    mctp_req_get_vendor_t:              MCTP_CTRL_CMD_GET_VENDOR_MESSAGE_SUPPORT, \
    mctp_req_resolve_eid_t:             MCTP_CTRL_CMD_RESOLVE_ENDPOINT_ID, \
    mctp_req_alloc_eid_t:               MCTP_CTRL_CMD_ALLOCATE_ENDPOINT_IDS, \
    mctp_req_routing_info_upd_t:        MCTP_CTRL_CMD_ROUTING_INFO_UPDATE, \
    mctp_req_get_routing_table_t:       MCTP_CTRL_CMD_GET_ROUTING_TABLE_ENTRIES, \
    mctp_req_prep_endpoint_discovery_t: MCTP_CTRL_CMD_PREP_ENDPOINT_DISCOVERY, \
    mctp_req_endpoint_discovery_t:      MCTP_CTRL_CMD_ENDPOINT_DISCOVERY, \
    mctp_req_discovery_notify_t:        MCTP_CTRL_CMD_DISCOVERY_NOTIFY, \
    mctp_req_get_network_id_t:          MCTP_CTRL_CMD_GET_NETWORK_ID, \
    mctp_req_query_hop_t:               MCTP_CTRL_CMD_QUERY_HOP, \
    mctp_req_resolve_uuid_t:            MCTP_CTRL_CMD_RESOLVE_UUID, \
    mctp_req_query_rate_limit_t:        MCTP_CTRL_CMD_QUERY_RATE_LIMIT, \
    mctp_req_request_tx_rate_limit_t:   MCTP_CTRL_CMD_REQUEST_TX_RATE_LIMIT, \
    mctp_req_upd_rate_limit_t:          MCTP_CTRL_CMD_UPDATE_RATE_LIMIT, \
    mctp_req_query_supp_if_t:           MCTP_CTRL_CMD_QUERY_SUPP_IF \
)

#endif // _MCTP_CONTROL_REQUEST_H_ 
