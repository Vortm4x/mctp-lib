#ifndef _MCTP_UTIL_PACKED_H_
#define _MCTP_UTIL_PACKED_H_

#define packed_enum \
    enum __attribute__ ((__packed__))

#define packed_struct \
    struct __attribute__ ((__packed__))

#define packed_union \
    union __attribute__ ((__packed__))

#endif // _MCTP_UTIL_PACKED_H_
