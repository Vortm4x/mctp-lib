#include <mctp/binding/serial.h>
#include <mctp/util/container_of.h>
#include <mctp/util/crc16.h>
#include <stdlib.h>
#include <string.h>


mctp_serial_t* mctp_serial_create()
{
    mctp_serial_t *serial = (mctp_serial_t *)calloc(1, sizeof(mctp_serial_t));

    serial->binding.type = MCTP_BINDING_TYPE_SERIAL;
    serial->binding.packet_tx = mctp_serial_packet_tx;

    return serial;
}

void mctp_serial_destroy(
    mctp_serial_t *serial
) {
    free(serial);
}

void mctp_serial_set_buffer_tx(
    mctp_serial_t *serial,
    mctp_serial_buffer_tx_t buffer_tx
) {
    serial->buffer_tx = buffer_tx;
}

mctp_binding_t *mctp_serial_get_binding(
    mctp_serial_t *serial
) {
    return &serial->binding;
}

void mctp_serial_buffer_map_init(
    mctp_serial_buffer_map_t *buffer_map,
    uint8_t *buffer_data,
    const size_t packet_len
) {
    buffer_map->header = (mctp_serial_header_t *)buffer_data;
    buffer_map->packet = (uint8_t *)(buffer_map->header + 1);
    buffer_map->trailer = (mctp_serial_trailer_t *)(buffer_map->packet + packet_len);
}

void mctp_serial_packet_tx(
    const mctp_binding_t *binding,
	const mctp_packet_t *packet
) {
    const size_t buffer_len =
        sizeof(mctp_serial_header_t) +
        sizeof(mctp_serial_trailer_t) +
        packet->len;

    uint8_t buffer_data[buffer_len];
    mctp_serial_buffer_map_t buffer_map = {};

    mctp_serial_buffer_map_init(&buffer_map, buffer_data, packet->len);

    buffer_map.header->framing_flag = MCTP_SERIAL_FRAME_FLAG;
    buffer_map.header->revision = MCTP_SERIAL_REVISION;
    buffer_map.header->byte_count = packet->len;

    memcpy(buffer_map.packet, packet->data, packet->len);

    uint16_t crc = MCTP_CRC16_INIT;
    crc = crc16_calc_byte(crc, buffer_map.header->revision);
    crc = crc16_calc_byte(crc, buffer_map.header->byte_count);
    crc = crc16_calc_block(crc, buffer_map.packet, packet->len);

    buffer_map.trailer->fcs_high = MCTP_CRC16_GET_HIGH(crc);
    buffer_map.trailer->fcs_low = MCTP_CRC16_GET_LOW(crc);
    buffer_map.trailer->framing_flag = MCTP_SERIAL_FRAME_FLAG;

    const mctp_serial_t *serial = container_of(
        binding, mctp_serial_t, binding
    );

    serial->buffer_tx(buffer_data, buffer_len);
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

    if(serial->rx.packet.len == serial->rx.next_pkt_byte)
    {
        serial->rx.state = MCTP_SERIAL_RX_STATE_FCS_HIGH;
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
            if(byte == MCTP_SERIAL_FRAME_FLAG)
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
            if(sizeof(serial->rx.packet.header) < byte && byte <= sizeof(serial->rx.packet.data))
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
            if(byte != MCTP_SERIAL_ESCAPE_FLAG)
            {
                mctp_serial_push_rx_data(serial, MCTP_SERIAL_ESCAPE_BYTE(byte));
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
            if(byte == MCTP_SERIAL_FRAME_FLAG && serial->rx.fcs_calc == serial->rx.fcs_read)
            {
                // TO DO: mctp_packet_rx
            }
            
            mctp_serial_reset_rx_ctx(serial);
        }
        break;
    }
}
