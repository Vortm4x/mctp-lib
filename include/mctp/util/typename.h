#ifndef _MCTP_UTIL_DECL_COMMON_H_
#define _MCTP_UTIL_DECL_COMMON_H_

#define _x_type_t_private(typename) typename##_t
#define _x_type_t(typename) _x_type_t_private(typename)

#define _x_node_t_private(typename) typename##_node_t
#define _x_node_t(typename) _x_node_t_private(typename)

#define _x_data_iface_private(method) typename##_##data_iface
#define _x_data_iface(typename) _x_data_iface_private(typename)

#define _x_method_private(typename, method) typename##_##method
#define _x_method(typename, method) _x_method_private(typename, method)

#endif //_MCTP_UTIL_DECL_COMMON_H_
