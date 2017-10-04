#include <cw_config.h>

void cw_master_process_cycle()
{
    sigset_t set;

    printf("master process start!\n");

    sigemptyset(&set);

    for ( ;; ) {
        sigsuspend(&set);
    }
}