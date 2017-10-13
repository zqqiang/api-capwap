#include <cw_config.h>
#include <cw_core.h>

void *
cw_pcalloc(cw_pool_t *pool, size_t size)
{
    void *p;

    p = calloc(1, size);

    return p;
}