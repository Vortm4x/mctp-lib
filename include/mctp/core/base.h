#ifndef _MCTP_CORE_BASE_H_
#define _MCTP_CORE_BASE_H_

#include <mctp/util/extern_c.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MCTP_BASE_MTU 64


typedef uint8_t mctp_eid_t;

#define MCTP_EID_NULL       ((mctp_eid_t)0x00)
#define MCTP_EID_BROADCAST  ((mctp_eid_t)0xFF)

typedef enum __attribute__ ((__packed__)) {
    MCTP_MSG_TYPE_CONTROL       = 0x00,
    MCTP_MSG_TYPE_PLDM          = 0x01,
    MCTP_MSG_TYPE_NCSI          = 0x02,
    MCTP_MSG_TYPE_ETHERNET      = 0x03,
    MCTP_MSG_TYPE_NVM_EXPRESS   = 0x04,
    MCTP_MSG_TYPE_SPDM          = 0x05,
    MCTP_MSG_TYPE_SECURED       = 0x06,
    MCTP_MSG_TYPE_VENDOR_PCI    = 0x7E,
    MCTP_MSG_TYPE_VENDOR_IANA   = 0x7F,
} mctp_msg_type_t;


typedef struct __attribute__ ((__packed__))
{
    mctp_msg_type_t type : 7;
    bool integrity_check : 1;
}
mctp_generic_header_t;

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

typedef union
{
    struct
    {
        mctp_eid_t eid;
        uint8_t tag     : 3;
        bool tag_owner  : 1;
        uint8_t         : 4;
    };
    uint16_t id : 12;
}
mctp_msg_ctx_t;


EXTERN_C_BEGIN

void mctp_generic_header_dump(
    const mctp_generic_header_t *header
);

EXTERN_C_END

#endif // _MCTP_CORE_BASE_H_
