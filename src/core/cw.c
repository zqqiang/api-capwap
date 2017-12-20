#include <cw_config.h>
#include <cw_core.h>
#include <cw.h>

static cw_int_t cw_process_options(cw_cycle_t *cycle);
static char *cw_set_worker_processes(cw_conf_t *cf, cw_command_t *cmd, void *conf);

static void *
cw_core_module_create_conf(cw_cycle_t *cycle)
{
    cw_core_conf_t  *ccf = NULL;

    ccf = cw_pcalloc(cycle->pool, sizeof(cw_core_conf_t));
    if (ccf == NULL) {
        return NULL;
    }

    cw_log_debug(cycle->log, "core create conf");

    return ccf;
}

static char *
cw_core_module_init_conf(cw_cycle_t *cycle, void *conf)
{
    cw_log_debug(cycle->log, "core init conf");

    return CW_CONF_OK;
}

static cw_command_t  cw_core_commands[] = {

    { cw_string("worker_processes"),
      CW_MAIN_CONF|CW_DIRECT_CONF|CW_CONF_TAKE1,
      cw_set_worker_processes,
      0,
      0,
      NULL },

      cw_null_command
};

static cw_core_module_t  cw_core_module_ctx = {
    cw_string("core"),
    cw_core_module_create_conf,
    cw_core_module_init_conf
};

cw_module_t  cw_core_module = {
    CW_MODULE_V1,
    &cw_core_module_ctx,                  /* module context */
    cw_core_commands,                     /* module directives */
    CW_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    CW_MODULE_V1_PADDING
};

static u_char      *cw_conf_file;

int cw_cdecl
main(int argc, char *const *argv)
{
    cw_log_t *log = NULL;
    cw_cycle_t *cycle = NULL, init_cycle;

    cw_pid = cw_getpid();

    log = cw_log_init();
    if (log == NULL) {
        return 1;
    }

    cw_memzero(&init_cycle, sizeof(cw_cycle_t));
    init_cycle.log = log;
    cw_cycle = &init_cycle;

    init_cycle.pool = cw_create_pool(1024, log);
    if (init_cycle.pool == NULL) {
        return 1;
    }

    if (cw_process_options(&init_cycle) != CW_OK) {
        return 1;
    }

    if (cw_os_init(log) != CW_OK) {
        return 1;
    }



    if (cw_preinit_modules() != CW_OK) {
        return 1;
    }

    cycle = cw_init_cycle(&init_cycle);
    if (NULL == cycle) {
        return 1;
    }

    cw_cycle = cycle;

    if (cw_init_signals(cycle->log) != CW_OK) {
        return 1;
    }

    cw_master_process_cycle(cycle);

    return CW_OK;
}

static cw_int_t
cw_process_options(cw_cycle_t *cycle)
{
    if (cw_conf_file) {
        cycle->conf_file.len = cw_strlen(cw_conf_file);
        cycle->conf_file.data = cw_conf_file;

    } else {
        cw_str_set(&cycle->conf_file, CW_CONF_PATH);
    }

    return CW_OK;
}

static char *
cw_set_worker_processes(cw_conf_t *cf, cw_command_t *cmd, void *conf)
{
    cw_str_t        *value;
    cw_core_conf_t  *ccf;

    ccf = (cw_core_conf_t *) conf;

    if (ccf->worker_processes != CW_CONF_UNSET) {
        return "is duplicate";
    }

    value = cf->args->elts;

    if (cw_strcmp(value[1].data, "auto") == 0) {
        ccf->worker_processes = cw_ncpu;
        return CW_CONF_OK;
    }

    ccf->worker_processes = cw_atoi(value[1].data, value[1].len);

    if (ccf->worker_processes == CW_ERROR) {
        return "invalid value";
    }

    return CW_CONF_OK;
}