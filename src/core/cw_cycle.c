#include <cw_config.h>
#include <cw_core.h>

volatile cw_cycle_t  *cw_cycle;

cw_uint_t             cw_dump_config;

static void
cw_destroy_cycle_pools(cw_conf_t *conf)
{
    cw_destroy_pool(conf->temp_pool);
    cw_destroy_pool(conf->pool);
}

cw_cycle_t *
cw_init_cycle(cw_cycle_t *old_cycle)
{
    void                *rv;
    char               **senv = NULL;
    cw_uint_t           i = 0;
    cw_log_t           *log;
    cw_cycle_t *cycle = NULL;
    cw_conf_t           conf;
    cw_pool_t          *pool;
    cw_core_module_t   *module;

    log = old_cycle->log;

    pool = cw_create_pool(CW_CYCLE_POOL_SIZE, log);
    if (pool == NULL) {
        return NULL;
    }
    pool->log = log;

    cycle = cw_pcalloc(pool, sizeof(cw_cycle_t));
    if (cycle == NULL) {
        cw_destroy_pool(pool);
        return NULL;
    }

    cycle->pool = pool;
    cycle->log = old_cycle->log;

    cycle->conf_ctx = cw_pcalloc(pool, cw_max_module * sizeof(void *));
    if (cycle->conf_ctx == NULL) {
        cw_destroy_pool(pool);
        return NULL;
    }

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = cw_pnalloc(pool, old_cycle->conf_file.len + 1);
    if (cycle->conf_file.data == NULL) {
        cw_destroy_pool(pool);
        return NULL;
    }
    cw_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);



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
                cw_destroy_pool(pool);
                return NULL;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }

    cw_memzero(&conf, sizeof(cw_conf_t));
    conf.args = cw_array_create(pool, 10, sizeof(cw_str_t));
    if (conf.args == NULL) {
        cw_destroy_pool(pool);
        return NULL;
    }

    conf.temp_pool = cw_create_pool(CW_CYCLE_POOL_SIZE, log);
    if (conf.temp_pool == NULL) {
        cw_destroy_pool(pool);
        return NULL;
    }

    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = CW_CORE_MODULE;
    conf.cmd_type = CW_MAIN_CONF;

    if (cw_conf_param(&conf) != CW_CONF_OK) {
        environ = senv;
        cw_destroy_cycle_pools(&conf);
        return NULL;
    }

    if (cw_conf_parse(&conf, &cycle->conf_file) != CW_CONF_OK) {
        environ = senv;
        cw_destroy_cycle_pools(&conf);
        return NULL;
    }



    if (cw_open_listening_sockets(cycle) != CW_OK) {
        goto failed;
    }

    return cycle;

failed:

    return NULL;
}