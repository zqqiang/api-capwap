#include <cw_config.h>
#include <cw_core.h>

static cw_str_t err_levels[] = {
    cw_null_string,
    cw_string("emerg"),
    cw_string("alert"),
    cw_string("crit"),
    cw_string("error"),
    cw_string("warn"),
    cw_string("notice"),
    cw_string("info"),
    cw_string("debug")
};

void
cw_log_error_core(cw_uint_t level, cw_log_t *log, cw_err_t err,
    const char *fmt, ...)
{
    u_char *p = NULL, *last = NULL;
    p = cw_slprintf(p, last, " [%V] ", &err_levels[level]);

    /* pid#tid */
    p = cw_slprintf(p, last, "%P#" CW_TID_T_FMT ": ",
                    cw_log_pid, cw_log_tid);
}