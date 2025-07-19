#ifndef _MCTP_CONTROL_TYPES_H_
#define _MCTP_CONTROL_TYPES_H_

#include <mctp/core/base.h>
#include <mctp/core/medium_info.h>
#include <mctp/control/constants.h>


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

#endif // _MCTP_CONTROL_TYPES_H_
