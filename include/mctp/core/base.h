#ifndef _MCTP_CORE_BASE_H_
#define _MCTP_CORE_BASE_H_

#include <mctp/util/packed.h>
#include <mctp/util/bitfield.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>


#define MCTP_BASE_MTU 64


typedef uint8_t mctp_eid_t;

#define MCTP_EID_NULL       ((mctp_eid_t)0x00)
#define MCTP_EID_BROADCAST  ((mctp_eid_t)0xFF)


typedef packed_enum 
{
    MCTP_MSG_TYPE_CONTROL       = 0x00,
    MCTP_MSG_TYPE_PLDM          = 0x01,
    MCTP_MSG_TYPE_NCSI          = 0x02,
    MCTP_MSG_TYPE_ETHERNET      = 0x03,
    MCTP_MSG_TYPE_NVM_EXPRESS   = 0x04,
    MCTP_MSG_TYPE_SPDM          = 0x05,
    MCTP_MSG_TYPE_SECURED       = 0x06,
    MCTP_MSG_TYPE_VENDOR_PCI    = 0x7E,
    MCTP_MSG_TYPE_VENDOR_IANA   = 0x7F,
}
mctp_msg_type_t;


#define MCTP_BASE_MASK_IC           0x01
#define MCTP_BASE_MASK_MSG_TYPE     0x7F

#define MCTP_BASE_OFFSET_IC         7
#define MCTP_BASE_OFFSET_MSG_TYPE   0


#define MCTP_BASE_GET_IC(flags) \
    (bool)BITFIELD_GET(flags, MCTP_BASE_OFFSET_IC, MCTP_BASE_MASK_IC)

#define MCTP_BASE_GET_MSG_TYPE(flags) \
    (mctp_msg_type_t)BITFIELD_GET(flags, MCTP_BASE_OFFSET_MSG_TYPE, MCTP_BASE_MASK_MSG_TYPE)


#define MCTP_BASE_SET_IC(flags, val) \
    (uint8_t)BITFIELD_SET(flags, val, MCTP_BASE_OFFSET_IC, MCTP_BASE_MASK_IC)

#define MCTP_BASE_SET_MSG_TYPE(flags, val) \
    (uint8_t)BITFIELD_SET(flags, val, MCTP_BASE_OFFSET_MSG_TYPE, MCTP_BASE_MASK_MSG_TYPE)

#endif // _MCTP_CORE_BASE_H_
