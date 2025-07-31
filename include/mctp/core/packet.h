#ifndef _MCTP_CORE_PACKET_H_
#define _MCTP_CORE_PACKET_H_

#include <mctp/core/base.h>
#include <mctp/util/packed.h>
#include <mctp/util/extern_c.h>
#include <assert.h>

typedef packed_struct
{
    uint8_t ver         : 4;
    uint8_t             : 4;
    mctp_eid_t dest;
    mctp_eid_t src;
    uint8_t tag         : 3;
    bool to             : 1;
    uint8_t seq         : 2;
    bool eom            : 1;
    bool som            : 1;
}
mctp_hdr_t;

#define MCTP_PKT_HDR_SIZE (sizeof(mctp_hdr_t))

static_assert(MCTP_PKT_HDR_SIZE == 4);


#define MCTP_PKT_MAX_SIZE (MCTP_PKT_HDR_SIZE + MCTP_BASE_MTU)
#define MCTP_PKT_MIN_SIZE (MCTP_PKT_HDR_SIZE)

typedef packed_union
{
    packed_struct
    {
        mctp_hdr_t header;
        uint8_t payload[MCTP_BASE_MTU];
    };
    uint8_t data[MCTP_PKT_MAX_SIZE];
}
mctp_io_pkt_t;

static_assert(sizeof(mctp_io_pkt_t) == MCTP_PKT_MAX_SIZE);


typedef struct mctp_pkt_t
{
    mctp_io_pkt_t io;
    size_t len;
}
mctp_pkt_t;


#endif // _MCTP_CORE_PACKET_H_