#ifndef _CW_LOG_H_INCLUDED_
#define _CW_LOG_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

#define cw_log_fatal zlog_fatal
#define cw_log_error zlog_error
#define cw_log_warn zlog_warn
#define cw_log_notice zlog_notice
#define cw_log_info zlog_info
#define cw_log_debug zlog_debug

#endif /* _CW_LOG_H_INCLUDED_ */