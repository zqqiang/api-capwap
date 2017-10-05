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

    printf("master process start!\n");

    sigemptyset(&set);

    for ( ;; ) {

        sigsuspend(&set);

        if (!live && (cw_terminate || cw_quit)) {
            cw_master_process_exit();
        }

        if (cw_terminate) {
            printf("%s\n", "do something terminate...");
            continue;
        }

        if (cw_quit) {
            printf("%s\n", "do something quit...");
            continue;
        }

    }
}

static void
cw_master_process_exit()
{
    printf("%s\n", "master process exit");

    exit(0);
}