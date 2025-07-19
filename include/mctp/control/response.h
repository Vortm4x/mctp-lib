#ifndef _MCTP_CONTROL_RESPONSE_H_
#define _MCTP_CONTROL_RESPONSE_H_

#include <mctp/control/constants.h>
#include <mctp/control/types.h>


#define MCTP_CTRL_RESP_LAYOUT(...)          \
    struct __attribute__ ((__packed__))     \
    {                                       \
        struct __attribute__ ((__packed__)) \
        {                                   \
            int MCTB_ZERO_PAYLOAD_STUB[0];  \
        };                                  \
        __VA_ARGS__                         \
    }


typedef MCTP_CTRL_RESP_LAYOUT()
mctp_resp_error_t;


// MCTP_CTRL_CMD_SET_ENDPOINT_ID
typedef MCTP_CTRL_RESP_LAYOUT(
    mctp_eid_alloc_status_t eid_alloc_status    : 2;
    uint8_t                                     : 2;
    mctp_eid_assign_status_t eid_assign_status  : 2;
    uint8_t                                     : 2;
    mctp_eid_t eid_setting;
    uint8_t eid_pool_size;
)
mctp_resp_set_endpoint_id_t;


// MCTP_CTRL_CMD_GET_ENDPOINT_ID
typedef MCTP_CTRL_RESP_LAYOUT(
    mctp_eid_t eid;
    mctp_eid_type_t eid_type            : 2;
    uint8_t                             : 2;
    mctp_endpoint_type_t endpoint_type  : 2;
    uint8_t                             : 2;
    uint8_t medium_info;
)
mctp_resp_get_endpoint_id_t;


// MCTP_CTRL_CMD_GET_ENDPOINT_UUID
typedef MCTP_CTRL_RESP_LAYOUT(
    uint8_t uuid[16];
)
mctp_resp_get_endpoint_uuid_t;


// MCTP_CTRL_CMD_GET_VERSION_SUPPORT
typedef MCTP_CTRL_RESP_LAYOUT(
    uint8_t version_count;
    mctp_ver_t version[];
)
mctp_resp_get_mctp_ver_t;


// MCTP_CTRL_CMD_GET_MESSAGE_TYPE_SUPPORT
typedef MCTP_CTRL_RESP_LAYOUT(
    uint8_t msg_type_count;
    mctp_msg_type_t msg_types[];
)
mctp_resp_get_msg_type_t;


// MCTP_CTRL_CMD_GET_VENDOR_MESSAGE_SUPPORT
typedef MCTP_CTRL_RESP_LAYOUT(
    uint8_t vendor_id_selector;
)
mctp_resp_get_vendor_t;


// MCTP_CTRL_CMD_RESOLVE_ENDPOINT_ID
typedef MCTP_CTRL_RESP_LAYOUT(

    mctp_eid_t bridge_eid;
    uint8_t physical_address[];
)
mctp_resp_resolve_eid_t;


// MCTP_CTRL_CMD_ALLOCATE_ENDPOINT_IDS
typedef MCTP_CTRL_RESP_LAYOUT(
    mctp_eid_assign_status_t eid_assign_status  : 2;
    uint8_t                                     : 6;
    uint8_t eid_pool_size;
    mctp_eid_t start_eid;
)
mctp_resp_alloc_eid_t;


// MCTP_CTRL_CMD_ROUTING_INFO_UPDATE
typedef MCTP_CTRL_RESP_LAYOUT()
mctp_resp_routing_info_upd_t;


// MCTP_CTRL_CMD_GET_ROUTING_TABLE_ENTRIES
typedef MCTP_CTRL_RESP_LAYOUT(
    uint8_t next_entry_handle;
    uint8_t routing_table_entry_count;
    mctp_routing_table_entry_t routing_table_entry;
)
mctp_resp_get_routing_table_t;


// MCTP_CTRL_CMD_PREP_ENDPOINT_DISCOVERY
typedef MCTP_CTRL_RESP_LAYOUT()
mctp_resp_prep_endpoint_discovery_t;


// MCTP_CTRL_CMD_ENDPOINT_DISCOVERY
typedef MCTP_CTRL_RESP_LAYOUT()
mctp_resp_endpoint_discovery_t;


// MCTP_CTRL_CMD_DISCOVERY_NOTIFY
typedef MCTP_CTRL_RESP_LAYOUT()
mctp_resp_discovery_notify_t;


// MCTP_CTRL_CMD_GET_NETWORK_ID
typedef MCTP_CTRL_RESP_LAYOUT(
    uint8_t network_id[16];
)
mctp_resp_get_network_id_t;


// MCTP_CTRL_CMD_QUERY_HOP
typedef MCTP_CTRL_RESP_LAYOUT(
    mctp_eid_t next_bridge;
    mctp_msg_type_t msg_type;
    uint16_t maxt_tu_in;
    uint16_t maxt_tu_out;
)
mctp_resp_query_hop_t;


// MCTP_CTRL_CMD_RESOLVE_UUID
typedef MCTP_CTRL_RESP_LAYOUT(
    uint8_t next_entry_handle;
    uint8_t entries_count;
    mctp_resolve_uuid_entry_t first_entry;
)
mctp_resp_resolve_uuid_t;


// MCTP_CTRL_CMD_QUERY_RATE_LIMIT
typedef MCTP_CTRL_RESP_LAYOUT(
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
typedef MCTP_CTRL_RESP_LAYOUT(
    uint32_t eid_tx_burst : 24;
    uint32_t eid_tx_rate;
)
mctp_resp_request_tx_rate_limit_t;


// MCTP_CTRL_CMD_UPDATE_RATE_LIMIT
typedef MCTP_CTRL_RESP_LAYOUT()
mctp_resp_upd_rate_limit_t;


// MCTP_CTRL_CMD_QUERY_SUPP_IF
typedef MCTP_CTRL_RESP_LAYOUT(
    uint8_t interface_count;
    mctp_interface_t firts_intefrace;
)
mctp_resp_query_supp_if_t;


#define MCTP_CTRL_TRANSP_RESP_LAYOUT(...)   \
    MCTP_CTRL_RESP_LAYOUT(                  \
        __VA_ARGS__                         \
    )

#endif // _MCTP_CONTROL_RESPONSE_H_
