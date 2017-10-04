#include <cw_config.h>
#include <cw_core.h>

int cw_cdecl
main(int argc, char *const *argv)
{
    if (cw_init_signals() != CW_OK) {
        return 1;
    }

    cw_master_process_cycle();
    return 0;
}