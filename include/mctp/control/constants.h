#ifndef _MCTP_CONTROL_CONSTANTS_H_
#define _MCTP_CONTROL_CONSTANTS_H_

#define MCTP_MSG_TYPE_BASE_SPEC 0xFF


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

#endif // _MCTP_CONTROL_CONSTANTS_H_ 
