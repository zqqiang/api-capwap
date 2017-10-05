#include <cw_config.h>
#include <cw_core.h>

int cw_cdecl
main(int argc, char *const *argv)
{

    cw_cycle_t *cycle = NULL;

    cw_pid = cw_getpid();

    if (cw_init_signals() != CW_OK) {
        return 1;
    }

    cw_master_process_cycle(cycle);
    return 0;
}