#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <mctp/base.h>

typedef struct __attribute__ ((__packed__))
{
    uint8_t version             : 4;
    uint8_t                     : 4;
    mctp_eid_t destination;
    mctp_eid_t source;
    uint8_t message_tag         : 3;
    bool tag_owner              : 1;
    uint8_t packet_sequence     : 2;
    bool end_of_message         : 1;
    bool start_of_message       : 1;    
}
mctp_header_t;


#define MCTP_PACKET_SIZE(transaction_size, header_size, trail_size) \
    (header_size + transaction_size + trail_size)

#define MCTP_TRANSACTION_SIZE(payload_size) \
    (payload_size + sizeof(mctp_header_t))

#define MCTP_PAYLOAD_SIZE(transaction_size) \
    (transaction_size - sizeof(mctp_header_t))



#endif //TRANSPORT_H