#ifndef _MCTP_CORE_BASE_H_
#define _MCTP_CORE_BASE_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t mctp_eid_t;


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

#endif // _MCTP_CORE_BASE_H_ 
