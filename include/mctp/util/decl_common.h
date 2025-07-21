#ifndef _MCTP_UTIL_DECL_COMMON_H_
#define _MCTP_UTIL_DECL_COMMON_H_

#define _x_type_t_(typename) typename##_t
#define _x_type_t(typename) _x_type_t_(typename)

#define _x_node_t_(typename) typename##_node_t
#define _x_node_t(typename) _x_node_t_(typename)

#define _x_method_(typename, method) typename##_##method
#define _x_method(typename, method) _x_method_(typename, method)

#endif //_MCTP_UTIL_DECL_COMMON_H_
