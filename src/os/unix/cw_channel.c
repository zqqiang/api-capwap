#include <cw_config.h>
#include <cw_core.h>

void
cw_close_channel(cw_fd_t *fd, cw_log_t *log)
{
    if (close(fd[0]) == -1) {
        cw_log_error(log, "close() channel failed");
    }

    if (close(fd[1]) == -1) {
        cw_log_error(log, "close() channel failed");
    }
}