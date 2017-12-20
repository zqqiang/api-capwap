/*
 * Copyright (C) Bill Qiang
 * Copyright (C) Nietsnie, Inc.
 */


#include <cw_config.h>
#include <cw_core.h>
#include <cw.h>

cw_int_t   cw_ncpu;


cw_int_t
cw_os_init(cw_log_t *log)
{
    // cw_time_t  *tp;
    // cw_uint_t   n;

#if (CW_HAVE_OS_SPECIFIC_INIT)
    if (cw_os_specific_init(log) != CW_OK) {
        return CW_ERROR;
    }
#endif

    // if (cw_init_setproctitle(log) != CW_OK) {
    //     return CW_ERROR;
    // }

    // cw_pagesize = getpagesize();
    // cw_cacheline_size = CW_CPU_CACHE_LINE;

    // for (n = cw_pagesize; n >>= 1; cw_pagesize_shift++) { /* void */ }

#if (CW_HAVE_SC_NPROCESSORS_ONLN)
    if (cw_ncpu == 0) {
        cw_ncpu = sysconf(_SC_NPROCESSORS_ONLN);
    }
#endif

    if (cw_ncpu < 1) {
        cw_ncpu = 1;
    }

    // cw_cpuinfo();

    // if (getrlimit(RLIMIT_NOFILE, &rlmt) == -1) {
    //     cw_log_error(log, "getrlimit(RLIMIT_NOFILE) failed");
    //     return CW_ERROR;
    // }

    // cw_max_sockets = (cw_int_t) rlmt.rlim_cur;

// #if (CW_HAVE_INHERITED_NONBLOCK || CW_HAVE_ACCEPT4)
//     cw_inherited_nonblocking = 1;
// #else
//     cw_inherited_nonblocking = 0;
// #endif

    // tp = cw_timeofday();
    // srandom(((unsigned) cw_pid << 16) ^ tp->sec ^ tp->msec);

    return CW_OK;
}