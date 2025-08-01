#ifndef _MCTP_CORE_PACKET_P_H_
#define _MCTP_CORE_PACKET_P_H_

#include <mctp/core/packet.h>
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
mctp_pkt_hdr_t;

static_assert(sizeof(mctp_pkt_hdr_t) == 4);


#define MCTP_PKT_HDR_SIZE (sizeof(mctp_pkt_hdr_t))
#define MCTP_PKT_MAX_SIZE (MCTP_PKT_HDR_SIZE + MCTP_BASE_MTU)
#define MCTP_PKT_MIN_SIZE (MCTP_PKT_HDR_SIZE)


typedef packed_union
{
    packed_struct
    {
        mctp_pkt_hdr_t header;
        uint8_t body[MCTP_BASE_MTU];
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


EXTERN_C_BEGIN

mctp_pkt_t *mctp_pkt_create(
    void
);

void mctp_pkt_destroy(
    mctp_pkt_t *packet
);

EXTERN_C_END

#endif // _MCTP_CORE_PACKET_P_H_
