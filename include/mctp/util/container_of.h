#ifndef _MCTP_UTIL_CONTAINER_OF_H_
#define _MCTP_UTIL_CONTAINER_OF_H_

#include <stddef.h>
#include <stdint.h>


#define container_of(ptr, type, member) \
    (                                   \
        (type *)(                       \
            (uint8_t*)(                 \
                (                       \
                    const typeof(       \
                        ((type *)0      \
                    )->member) *        \
                )                       \
                (                       \
                    (ptr)               \
                )                       \
            )                           \
            - offsetof(type, member)    \
        )                               \
    )

#endif // _MCTP_UTIL_CONTAINER_OF_H_
