#include <mctp/binding/serial.h>
#include <mctp/core/mctp.h>
#include <mctp/util/container_of.h>
#include <mctp/util/crc16.h>
#include <mctp/util/alloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


mctp_serial_t* mctp_serial_create()
{
    mctp_serial_t *serial = _alloc(mctp_serial_t);

    serial->binding.type = MCTP_BINDING_TYPE_SERIAL;
    serial->binding.packet_tx = mctp_serial_packet_tx;

    return serial;
}

void mctp_serial_destroy(
    mctp_serial_t *serial
) {
    free(serial);
}

void mctp_serial_set_byte_tx(
    mctp_serial_t *serial,
    mctp_serial_byte_tx_t byte_tx
) {
    serial->byte_tx = byte_tx;
}

mctp_binding_t *mctp_serial_get_binding(
    mctp_serial_t *serial
) {
    return &serial->binding;
}

void mctp_serial_buffer_tx(
    const mctp_serial_t *serial,
    const uint8_t buffer_data[],
    const size_t buffer_len
) {
    for(size_t i = 0; i < buffer_len; ++i) 
    {
        serial->byte_tx(buffer_data[i]);
    }
}

void mctp_serial_escaped_buffer_tx(
    const mctp_serial_t *serial,
    const uint8_t buffer_data[],
    const size_t buffer_len
) {
    for(size_t i = 0; i < buffer_len; ++i) 
    {
        switch (buffer_data[i])
        {
            case MCTP_SERIAL_FRAME_FLAG:
            case MCTP_SERIAL_ESCAPE_FLAG:
                serial->byte_tx(MCTP_SERIAL_ESCAPE_FLAG);
                serial->byte_tx(MCTP_SERIAL_ESCAPE_BYTE(buffer_data[i]));
                break;

            default:
                serial->byte_tx(buffer_data[i]);
                break;
        }
    }
}

void mctp_serial_frame_tx(
    const mctp_serial_t *serial,
    const mctp_serial_header_t *header,
	const mctp_packet_t *packet,
    const mctp_serial_trailer_t *trailer
) {
    mctp_serial_buffer_tx(serial, header->data, sizeof(header->data));
    mctp_serial_escaped_buffer_tx(serial, packet->data, packet->len);
    mctp_serial_buffer_tx(serial, trailer->data, sizeof(trailer->data));
}

void mctp_serial_packet_tx(
    const mctp_binding_t *binding,
	const mctp_packet_t *packet
) {
    const mctp_serial_header_t header = {
        .framing_flag = MCTP_SERIAL_FRAME_FLAG,
        .revision = MCTP_SERIAL_REVISION,
        .byte_count = packet->len
    };

    uint16_t crc = MCTP_CRC16_INIT;

    crc = crc16_calc_byte(crc, header.revision);
    crc = crc16_calc_byte(crc, header.byte_count);
    crc = crc16_calc_block(crc, packet->data, packet->len);

    const mctp_serial_trailer_t trailer = {
        .fcs_high = MCTP_CRC16_GET_HIGH(crc),
        .fcs_low = MCTP_CRC16_GET_LOW(crc),
        .framing_flag = MCTP_SERIAL_FRAME_FLAG
    };

    const mctp_serial_t *serial = container_of(
        binding, mctp_serial_t, binding
    );

    mctp_serial_frame_tx(serial, &header, packet, &trailer);
}

void mctp_serial_reset_rx_ctx(
    mctp_serial_t* serial
) {
    memset(&serial->rx, 0, sizeof(&serial->rx));
    serial->rx.state = MCTP_SERIAL_RX_STATE_SYNC_START;
}

void mctp_serial_push_rx_data(
    mctp_serial_t *serial,
	const uint8_t byte
) {
    serial->rx.fcs_calc = crc16_calc_byte(serial->rx.fcs_calc, byte);
    serial->rx.packet.data[serial->rx.next_pkt_byte] = byte;
    serial->rx.next_pkt_byte++;

    if (serial->rx.packet.len == serial->rx.next_pkt_byte)
    {
        serial->rx.state = MCTP_SERIAL_RX_STATE_FCS_HIGH;
    }
    else
    {
        serial->rx.state = MCTP_SERIAL_RX_STATE_DATA;
    }
}

void mctp_serial_byte_rx(
    const mctp_binding_t *binding,
	const uint8_t byte
) {
    mctp_serial_t *serial = container_of(
        binding, mctp_serial_t, binding
    );

    switch(serial->rx.state)
    {
        case MCTP_SERIAL_RX_STATE_SYNC_START:
        {
            if (byte == MCTP_SERIAL_FRAME_FLAG)
            {
                serial->rx.fcs_calc = MCTP_CRC16_INIT;
                serial->rx.state = MCTP_SERIAL_RX_STATE_REVISION;
            }
        }
        break;

        case MCTP_SERIAL_RX_STATE_REVISION:
        {
            switch (byte) {
                case MCTP_SERIAL_REVISION:
                    serial->rx.fcs_calc = crc16_calc_byte(serial->rx.fcs_calc, byte);
                    serial->rx.state = MCTP_SERIAL_RX_STATE_PKT_LEN;
                break;
            
                case MCTP_SERIAL_FRAME_FLAG:
                break;

                default:
                    mctp_serial_reset_rx_ctx(serial);
                    break;
            }
        }
        break;

        case MCTP_SERIAL_RX_STATE_PKT_LEN:
        {
            if (MCTP_PKT_HDR_SIZE < byte && byte <= MCTP_PKT_MAX_SIZE)
            {
                serial->rx.fcs_calc = crc16_calc_byte(serial->rx.fcs_calc, byte);
                serial->rx.packet.len = byte;
                serial->rx.next_pkt_byte = 0;
                serial->rx.state = MCTP_SERIAL_RX_STATE_DATA;
            }
            else
            {
                mctp_serial_reset_rx_ctx(serial);
            }
        }
        break;

        case MCTP_SERIAL_RX_STATE_ESCAPE:
        {
            switch (MCTP_SERIAL_ESCAPE_BYTE(byte)) {
                case MCTP_SERIAL_ESCAPE_FLAG:
                case MCTP_SERIAL_FRAME_FLAG:
                    mctp_serial_push_rx_data(serial, MCTP_SERIAL_ESCAPE_BYTE(byte));
                    break;
                default:
                    mctp_serial_reset_rx_ctx(serial);
                    break;
            }
        }
        break;

        case MCTP_SERIAL_RX_STATE_DATA:
        {
            if (byte != MCTP_SERIAL_ESCAPE_FLAG)
            {
                mctp_serial_push_rx_data(serial, byte);
            }
            else
            {
                serial->rx.state = MCTP_SERIAL_RX_STATE_ESCAPE;
            }
        }
        break;

        case MCTP_SERIAL_RX_STATE_FCS_HIGH:
        {
            MCTP_CRC16_SET_HIGH(serial->rx.fcs_read, byte);
            serial->rx.state = MCTP_SERIAL_RX_STATE_FCS_LOW;
        }
        break;

        case MCTP_SERIAL_RX_STATE_FCS_LOW:
        {
            MCTP_CRC16_SET_LOW(serial->rx.fcs_read, byte);
            serial->rx.state = MCTP_SERIAL_RX_STATE_SYNC_END;
        }
        break;

        case MCTP_SERIAL_RX_STATE_SYNC_END:
        {
            if (byte == MCTP_SERIAL_FRAME_FLAG && serial->rx.fcs_calc == serial->rx.fcs_read)
            {
                mctp_packet_rx(binding->bus, &serial->rx.packet);
            }
            
            mctp_serial_reset_rx_ctx(serial);
        }
        break;
    }
}

void mctp_serial_header_dump(
    const mctp_serial_header_t *header
) {
    printf("MCTP Serial binding header\n");
    printf("frame flag: 0x%02X\n",  header->framing_flag);
    printf("revision:   0x%02X\n",  header->revision);
    printf("byte count: 0x%dX\n",  header->byte_count);
    printf("\n");
}

void mctp_serial_trailer_dump(
    const mctp_serial_trailer_t *trailer
) {
    printf("MCTP Serial binding trailer\n");
    printf("FCS high:   0x%02X\n",  trailer->fcs_high);
    printf("FCS low:    0x%02X\n",  trailer->fcs_low);
    printf("frame flag: 0x%02X\n",  trailer->framing_flag);
    printf("\n");
}
