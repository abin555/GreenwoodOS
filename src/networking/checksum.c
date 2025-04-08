#include "stdint.h"
#include "checksum.h"

uint16_t NetChecksum(const uint8_t *data, const uint8_t *end)
{
    uint32_t sum = NetChecksumAcc(data, end, 0);
    return NetChecksumFinal(sum);
}

uint32_t NetChecksumAcc(const uint8_t *data, const uint8_t *end, uint32_t sum)
{
    uint32_t len = end - data;
    uint16_t *p = (uint16_t *)data;

    while (len > 1)
    {
        sum += *p++;
        len -= 2;
    }

    if (len)
    {
        sum += *(uint8_t *)p;
    }

    return sum;
}

uint16_t NetChecksumFinal(uint32_t sum)
{
    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);

    uint16_t temp = ~sum;
    return ((temp & 0x00ff) << 8) | ((temp & 0xff00) >> 8); // TODO - shouldn't swap this twice
}