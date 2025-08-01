#ifndef _MCTP_UTIL_BUFFER_H_
#define _MCTP_UTIL_BUFFER_H_

#include <stdint.h>
#include <stddef.h>

typedef struct buffer_t
{
    uint8_t *data;
    size_t len;
}
buffer_t;

#endif // _MCTP_UTIL_BUFFER_H_
