#ifndef _MCTP_UTIL_BITFIELD_H_
#define _MCTP_UTIL_BITFIELD_H_

#define BITFIELD_GET(field, off, mask) \
    ((field >> off) & mask)

#define BITFIELD_SET(field, val, off, mask) \
    ((field & ~(mask << off)) | (((val) & mask) << off))

#endif // _MCTP_UTIL_BITFIELD_H_