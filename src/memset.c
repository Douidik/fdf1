#include "util.h"
#include <stdint.h>

void *fdf_memset(void *dst, int value, size_t size)
{
    size_t i;
    const uint8_t byte = (uint8_t)value;
    const uint64_t qword = (uint64_t)byte << 0 | (uint64_t)byte << 8 | (uint64_t)byte << 16 | (uint64_t)byte << 24 |
                           (uint64_t)byte << 32 | (uint64_t)byte << 40 | (uint64_t)byte << 48 | (uint64_t)byte << 56;

    i = 0;
    while (i + sizeof(uint64_t) <= size)
    {
	(*(uint64_t *)(dst + i)) = qword;
        i += sizeof(uint64_t);
    }
    while (i + sizeof(uint8_t) <= size)
    {
	(*(uint8_t *)(dst + i)) = byte;
        i += sizeof(uint8_t);
    }
    return dst;
}
