#include <cw_config.h>
#include <cw_core.h>

volatile cw_cycle_t  *cw_cycle;

cw_cycle_t *
cw_init_cycle(cw_cycle_t *old_cycle)
{
    void                *rv;
    cw_uint_t           i = 0;
    cw_cycle_t *cycle = NULL;
    cw_core_module_t   *module;

    cycle = malloc(sizeof(cw_cycle_t)); // todo: suppor memory pool

    cycle->log = old_cycle->log;

    if (cw_cycle_modules(cycle) != CW_OK) {
        return NULL;
    }

    for (i = 0; cycle->modules[i]; i++) {
        if (cycle->modules[i]->type != CW_CORE_MODULE) {
            continue;
        }

        module = cycle->modules[i]->ctx;

        if (module->create_conf) {
            rv = module->create_conf(cycle);
            if (rv == NULL) {
                // cw_destroy_pool(pool);
                return NULL;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }

    if (cw_open_listening_sockets(cycle) != CW_OK) {
        goto failed;
    }

    return cycle;

failed:

    return NULL;
}