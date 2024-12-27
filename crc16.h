
#ifndef _CRC16_H
#define _CRC16_H

#include <stdint.h>
#include <stdlib.h>

void crc16_init(void);

uint16_t crc16_update(const uint8_t *data, size_t data_len);

uint16_t crc16_final(void);

uint16_t crc16_calc(const uint8_t *data, size_t data_len);

#endif //_CRC16_H

