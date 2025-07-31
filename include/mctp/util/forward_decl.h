#ifndef _MCTP_UTIL_FORWARD_DECL_H_
#define _MCTP_UTIL_FORWARD_DECL_H_

#define forward_decl(type)      \
    struct type;                \
    typedef struct type type

#endif // _MCTP_UTIL_FORWARD_DECL_H_