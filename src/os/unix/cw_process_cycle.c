#include <cw_config.h>
#include <cw_core.h>

static void cw_start_worker_processes(cw_cycle_t *cycle, cw_int_t n, cw_int_t type);
static void cw_worker_process_cycle(cw_cycle_t *cycle, void *data);
static void cw_master_process_exit();

cw_pid_t     cw_pid;

sig_atomic_t  cw_terminate;
sig_atomic_t  cw_quit;

void cw_master_process_cycle(cw_cycle_t *cycle)
{
    sigset_t set;
    cw_uint_t live = 1;
    // cw_core_conf_t *ccf = NULL;

    sigemptyset(&set);

    cw_start_worker_processes(cycle, /* ccf->worker_processes */ 1, CW_PROCESS_RESPAWN);

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

static void
cw_start_worker_processes(cw_cycle_t *cycle, cw_int_t n, cw_int_t type)
{
    cw_int_t i = 0;

    cw_log_notice(cycle->log, "start worker processes");

    for (i = 0; i < n; ++i) {
        cw_spawn_process(cycle, cw_worker_process_cycle, 
            (void *)(intptr_t)i, "worker process", type);
    }
}

static void
cw_worker_process_cycle(cw_cycle_t *cycle, void *data)
{
    cw_int_t worker = (intptr_t) data;

    for ( ;; ) {
        cw_log_notice(cycle->log, "worker cycle");

        cw_process_events_and_timers(cycle);

        if (cw_terminate) {
            cw_log_notice(cycle->log, "exiting");
            cw_worker_process_exit(cycle);
        }
    }
}

static void
cw_worker_process_exit(cw_cycle_t *cycle)
{

}