#ifndef _CW_LOG_H_INCLUDED_
#define _CW_LOG_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

struct cw_log_s {
    zlog_category_t *c;
};

cw_log_t *cw_log_init();
void cw_log_fini();


#define cw_log_fatal(log, ...) \
    zlog_fatal(log->c, __VA_ARGS__)

#define cw_log_error(log, ...) \
    zlog_error(log->c, __VA_ARGS__)

#define cw_log_warn(log, ...) \
    zlog_warn(log->c, __VA_ARGS__)

#define cw_log_notice(log, ...) \
    zlog_notice(log->c, __VA_ARGS__)

#define cw_log_info(log, ...) \
    zlog_info(log->c, __VA_ARGS__)

#define cw_log_debug(log, ...) \
    zlog_debug(log->c, __VA_ARGS__)


#endif /* _CW_LOG_H_INCLUDED_ */