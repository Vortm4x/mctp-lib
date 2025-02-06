#ifndef _MCTP_UTIL_CRC16_H_
#define _MCTP_UTIL_CRC16_H_

#include <stdint.h>
#include <stddef.h>

#define MCTP_CRC16_INIT 0xFFFF

#define MCTP_CRC16_HIGH(crc) ((uint8_t)(crc >> 8))
#define MCTP_CRC16_LOW(crc)  ((uint8_t)(crc >> 0))


uint16_t crc16_calc_byte(uint16_t crc, uint8_t byte);
uint16_t crc16_calc_block(uint16_t crc, uint8_t block[], size_t block_len);

#endif // _MCTP_UTIL_CRC16_H_
