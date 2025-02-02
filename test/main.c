#include <mctp/control/message.h>


#include <math.h>
#include <stdio.h>




int main() {
    mctp_req_get_msg_type_t payload = {
    };

    mctp_ctrl_payload_request_tx(payload, false);
    mctp_ctrl_empty_request_tx(MCTP_CTRL_CMD_GET_ENDPOINT_ID, false);

}
