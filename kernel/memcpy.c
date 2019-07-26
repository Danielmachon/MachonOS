#include <stddef.h>

void* memcpy(void *dst, void *src, size_t len)
{
    int x;
    for (x = 0; x < len; x++)
        *((char *) dst + x) = *((char *) src + x);
        
    return dst;
}
