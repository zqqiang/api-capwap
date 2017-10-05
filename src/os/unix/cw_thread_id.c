#include <cw_config.h>
#include <cw_core.h>

cw_tid_t
cw_thread_tid(void)
{
    return syscall(SYS_gettid);
}