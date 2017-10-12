#include <cw_config.h>
#include <cw_core.h>

volatile cw_cycle_t  *cw_cycle;

cw_cycle_t *
cw_init_cycle(cw_cycle_t *old_cycle)
{
    cw_cycle_t *cycle = NULL;

    cycle = malloc(sizeof(cw_cycle_t)); // todo: suppor memory pool

    cycle->log = old_cycle->log;

    if (cw_cycle_modules(cycle) != CW_OK) {
        return NULL;
    }

    return cycle;
}