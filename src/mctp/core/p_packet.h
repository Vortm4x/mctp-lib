#ifndef _MCTP_CORE_PACKET_P_H_
#define _MCTP_CORE_PACKET_P_H_

#include <mctp/core/p_packet.h>
#include <mctp/core/base.h>
#include <mctp/util/packed.h>
#include <mctp/util/extern_c.h>
#include <assert.h>


#define MCTP_PKT_HDR_VER 0x01


typedef packed_struct
{
    uint8_t version     : 4;
    uint8_t             : 4;
    mctp_eid_t dest;
    mctp_eid_t source;
    uint8_t tag         : 3;
    bool tag_owner      : 1;
    uint8_t seq_num     : 2;
    bool eom            : 1;
    bool som            : 1;
}
mctp_pkt_hdr_t;

static_assert(sizeof(mctp_pkt_hdr_t) == 4);


typedef uint8_t mctp_pkt_size_t;


#define MCTP_PKT_HDR_SIZE ((mctp_pkt_size_t)sizeof(mctp_pkt_hdr_t))
#define MCTP_PKT_MAX_SIZE ((mctp_pkt_size_t)(MCTP_PKT_HDR_SIZE + MCTP_BASE_MTU))
#define MCTP_PKT_MIN_SIZE ((mctp_pkt_size_t)(MCTP_PKT_HDR_SIZE))


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
    mctp_pkt_size_t len;
}
mctp_pkt_t;


EXTERN_C_BEGIN

mctp_pkt_t *mctp_pkt_create(
    void
);

mctp_pkt_t *mctp_pkt_clone(
    const mctp_pkt_t *packet
);

void mctp_pkt_destroy(
    mctp_pkt_t *packet
);

EXTERN_C_END

#endif // _MCTP_CORE_PACKET_P_H_
