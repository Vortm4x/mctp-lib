#include <mctp/core/message_queue.h>
#include <mctp/util/queue_impl.h>

_queue_type_impl(
    mctp_msgq,
    mctp_msg_ctx_t,
    _queue_value_deinit_stub,
    {}
)
