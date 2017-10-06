#include <cw_config.h>
#include <cw_core.h>

static void cw_master_process_exit();

cw_pid_t     cw_pid;

sig_atomic_t  cw_terminate;
sig_atomic_t  cw_quit;

void cw_master_process_cycle(cw_cycle_t *cycle)
{
    sigset_t set;
    cw_uint_t live = 1;

    sigemptyset(&set);

    for ( ;; ) {

        cw_log_debug(cycle->log, "sigsuspend");

        sigsuspend(&set);

        if (!live && (cw_terminate || cw_quit)) {
            cw_master_process_exit(cycle);
        }

        if (cw_terminate) {
            cw_log_info(cycle->log, "do something terminate...");
            continue;
        }

        if (cw_quit) {
            cw_log_info(cycle->log, "do something quit...");
            continue;
        }

    }
}

static void
cw_master_process_exit(cw_cycle_t *cycle)
{
    cw_log_info(cycle->log, "master process exit");

    cw_log_fini();

    free(cycle);
    cycle = NULL;

    exit(0);
}