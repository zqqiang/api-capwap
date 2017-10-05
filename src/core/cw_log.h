#ifndef _CW_LOG_H_INCLUDED_
#define _CW_LOG_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

#define CW_LOG_STDERR            0
#define CW_LOG_EMERG             1
#define CW_LOG_ALERT             2
#define CW_LOG_CRIT              3
#define CW_LOG_ERR               4
#define CW_LOG_WARN              5
#define CW_LOG_NOTICE            6
#define CW_LOG_INFO              7
#define CW_LOG_DEBUG             8

#define CW_LOG_DEBUG_EVENT       0x080

struct cw_log_s {

    cw_uint_t           log_level;

};

void cw_log_error_core(cw_uint_t level, cw_log_t *log, cw_err_t err,
    const char *fmt, ...);

#define cw_log_debug(level, log, args...)                                    \
    if ((log)->log_level & level)                                             \
        cw_log_error_core(CW_LOG_DEBUG, log, args)

#define cw_log_debug0(level, log, err, fmt)                                  \
        cw_log_debug(level, log, err, fmt)

#endif /* _CW_LOG_H_INCLUDED_ */