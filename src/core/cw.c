#include <cw_config.h>
#include <cw_core.h>

int cw_cdecl
main(int argc, char *const *argv)
{
    cw_log_t *log = NULL;
    cw_cycle_t *cycle = NULL, init_cycle = {0};

    cw_pid = cw_getpid();

    log = cw_log_init();
    if (log == NULL) {
        return 1;
    }

    init_cycle.log = log;
    cw_cycle = &init_cycle;

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