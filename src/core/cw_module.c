

cw_uint_t         cw_max_module;
static cw_uint_t  cw_modules_n;

cw_int_t
cw_preinit_modules(void)
{
    cw_uint_t  i;

    for (i = 0; cw_modules[i]; i++) {
        cw_modules[i]->index = i;
        cw_modules[i]->name = cw_module_names[i];
    }

    cw_modules_n = i;
    cw_max_module = cw_modules_n + CW_MAX_DYNAMIC_MODULES;

    return CW_OK;
}

cw_int_t
cw_cycle_modules(cw_cycle_t *cycle)
{
    cycle->modules = cw_pcalloc(cycle->pool, (cw_max_module + 1)
                                              * sizeof(cw_module_t *));
    if (cycle->modules == NULL) {
        return CW_ERROR;
    }

    cw_memcpy(cycle->modules, cw_modules,
               cw_modules_n * sizeof(cw_module_t *));

    cycle->modules_n = cw_modules_n;

    return CW_OK;
}