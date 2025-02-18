#ifndef _MCTP_UTIL_ALLOC_H_
#define _MCTP_UTIL_ALLOC_H_

#include <stdlib.h>


#define _alloc(type) (type *)calloc(1, sizeof(type))

#endif // _MCTP_UTIL_ALLOC_H_