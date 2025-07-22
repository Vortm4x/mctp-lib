#include <mctp/core/packet_queue.h>


#define typename mctp_pktq
#define queue_value_t mctp_packet_t *


#define queue_data_destroy(data) \
    mctp_pkt_destroy(data)


#include <mctp/util/queue_impl.h>
