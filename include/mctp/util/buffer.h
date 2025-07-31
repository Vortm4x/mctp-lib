#ifndef _MCTP_UTIL_BUFFER_H_
#define _MCTP_UTIL_BUFFER_H_

#include <stddef.h>
#include <stdint.h>


typedef struct
{
    union
    {
        uint8_t *data;
        void *vdata;
        char *cdata;
    };

    size_t size;
}
buffer_t;

#endif // _MCTP_UTIL_BUFFER_H_
