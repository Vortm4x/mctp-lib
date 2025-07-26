#ifndef _MCTP_UTIL_ALLOC_H_
#define _MCTP_UTIL_ALLOC_H_

#include <stdlib.h>


#define zalloc_n(type, n) (type *)calloc(n, sizeof(type))
#define zalloc(type) zalloc_n(type, 1)

#endif // _MCTP_UTIL_ALLOC_H_