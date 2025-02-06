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
    const mctp_serial_t *serial
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

    buffer_map.trailer->fcs_high = MCTP_CRC16_HIGH(crc);
    buffer_map.trailer->fcs_low = MCTP_CRC16_LOW(crc);
    buffer_map.trailer->framing_flag = MCTP_SERIAL_FRAME_FLAG;

    const mctp_serial_t *serial_binding = container_of(
        binding, mctp_serial_t, binding
    );

    serial_binding->buffer_tx(buffer_data, buffer_len);
}
