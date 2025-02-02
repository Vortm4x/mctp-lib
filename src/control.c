#include <mctp/control/message.h>
#include <stdio.h>
#include <stdlib.h>


void mctp_ctrl_request_tx(
    const mctp_ctrl_cmd_t command,
    const bool datagram,
    const uint8_t payload_data[],
    const size_t payload_len
) {
    static uint8_t curr_inst = 0;

    mctp_ctrl_header_t header = {
        .base = {
            .integrity_check = false,
            .type = MCTP_MSG_TYPE_CONTROL,
        },
        .instance = curr_inst++,
        .datagram = datagram,
        .request = true
    };

    uint8_t* message_data = NULL;
    size_t message_len = 0;

    {
        FILE* mctp_message = open_memstream((char**)&message_data, &message_len);

        fwrite(&header, sizeof(mctp_ctrl_header_t), 1, mctp_message);
        if (payload_data != NULL) {
            fwrite(payload_data, sizeof(uint8_t), payload_len, mctp_message);
        }

        fclose(mctp_message);
    }

    //TO DO: mctp_message_tx(message_data, message_len);

    free(message_data);
}
