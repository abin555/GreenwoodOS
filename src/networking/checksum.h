#ifndef NET_CHECKSUM_H
#define NET_CHECKSUM_H

uint16_t NetChecksum(const uint8_t *data, const uint8_t *end);
uint32_t NetChecksumAcc(const uint8_t *data, const uint8_t *end, uint32_t sum);
uint16_t NetChecksumFinal(uint32_t sum);

#endif