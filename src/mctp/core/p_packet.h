#ifndef _MCTP_CORE_PACKET_P_H_
#define _MCTP_CORE_PACKET_P_H_

#include <mctp/core/p_packet.h>
#include <mctp/core/base.h>
#include <mctp/util/bitfield.h>
#include <mctp/util/extern_c.h>
#include <assert.h>


#define MCTP_PKT_HDR_VER 0x01


typedef struct
{
    uint8_t version;
    mctp_eid_t dest;
    mctp_eid_t source;
    uint8_t flags;
}
mctp_hdr_t;

static_assert(sizeof(mctp_hdr_t) == 4);

#define MCTP_HDR_FLAG_OFFSET_SOM          7
#define MCTP_HDR_FLAG_OFFSET_EOM          6
#define MCTP_HDR_FLAG_OFFSET_SEQ_NUM      5
#define MCTP_HDR_FLAG_OFFSET_TAG_OWNER    3
#define MCTP_HDR_FLAG_OFFSET_MSG_TAG      0

#define MCTP_HDR_FLAG_MASK_SOM          0x1
#define MCTP_HDR_FLAG_MASK_EOM          0x1
#define MCTP_HDR_FLAG_MASK_SEQ_NUM      0x3
#define MCTP_HDR_FLAG_MASK_TAG_OWNER    0x1
#define MCTP_HDR_FLAG_MASK_MSG_TAG      0x7


#define MCTP_HDR_FLAG_GET_SOM(flags) \
    (bool)BITFIELD_GET(flags, MCTP_HDR_FLAG_OFFSET_SOM, MCTP_HDR_FLAG_MASK_SOM)

#define MCTP_HDR_FLAG_GET_EOM(flags) \
    (bool)BITFIELD_GET(flags, MCTP_HDR_FLAG_OFFSET_EOM, MCTP_HDR_FLAG_MASK_EOM)

#define MCTP_HDR_FLAG_GET_SEQ_NUM(flags) \
    (uint8_t)BITFIELD_GET(flags, MCTP_HDR_FLAG_OFFSET_SEQ_NUM, MCTP_HDR_FLAG_MASK_SEQ_NUM)

#define MCTP_HDR_FLAG_GET_TAG_OWNER(flags) \
    (bool)BITFIELD_GET(flags, MCTP_HDR_FLAG_OFFSET_TAG_OWNER, MCTP_HDR_FLAG_MASK_TAG_OWNER)

#define MCTP_HDR_FLAG_GET_MSG_TAG(flags) \
    (uint8_t)BITFIELD_GET(flags, MCTP_HDR_FLAG_OFFSET_MSG_TAG, MCTP_HDR_FLAG_MASK_MSG_TAG)


#define MCTP_HDR_FLAG_SET_SOM(flags, val) \
    (uint8_t)BITFIELD_SET(flags, val, MCTP_HDR_FLAG_OFFSET_SOM, MCTP_HDR_FLAG_MASK_SOM)

#define MCTP_HDR_FLAG_SET_EOM(flags, val) \
    (uint8_t)BITFIELD_SET(flags, val, MCTP_HDR_FLAG_OFFSET_EOM, MCTP_HDR_FLAG_MASK_EOM)

#define MCTP_HDR_FLAG_SET_SEQ_NUM(flags, val) \
    (uint8_t)BITFIELD_SET(flags, val, MCTP_HDR_FLAG_OFFSET_SEQ_NUM, MCTP_HDR_FLAG_MASK_SEQ_NUM)

#define MCTP_HDR_FLAG_SET_TAG_OWNER(flags, val) \
    (uint8_t)BITFIELD_SET(flags, val, MCTP_HDR_FLAG_OFFSET_TAG_OWNER, MCTP_HDR_FLAG_MASK_TAG_OWNER)

#define MCTP_HDR_FLAG_SET_MSG_TAG(flags, val) \
    (uint8_t)BITFIELD_SET(flags, val, MCTP_HDR_FLAG_OFFSET_MSG_TAG, MCTP_HDR_FLAG_MASK_MSG_TAG)


typedef uint8_t mctp_pkt_size_t;

#define MCTP_PKT_HDR_SIZE (4)
#define MCTP_PKT_MAX_SIZE (MCTP_PKT_HDR_SIZE + MCTP_BASE_MTU)
#define MCTP_PKT_MIN_SIZE (MCTP_PKT_HDR_SIZE)


typedef union
{
    struct
    {
        mctp_hdr_t header;
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
