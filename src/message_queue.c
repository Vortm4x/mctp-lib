#include <mctp/core/message_queue.h>
#include <mctp/util/queue/impl.h>

typedef mctp_message_t queue_value_t;

static void queue_value_destroy(
    queue_value_t* message
) {
    free(message->data);
}


_x_queue_value_iface(
    mctp_msgq,
    queue_value_t
) (
    queue_value_destroy
)

_x_queue_type_impl(
    mctp_msgq,
    queue_value_t
)
