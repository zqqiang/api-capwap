#ifndef _CW_PROCESS_H_INCLUDED_
#define _CW_PROCESS_H_INCLUDED_

typedef pid_t cw_pid_t;

#define cw_getpid getpid

#ifndef cw_log_pid
#define cw_log_pid  cw_pid
#endif

cw_int_t cw_init_signals();

extern cw_pid_t cw_pid;

#endif /* _CW_PROCESS_H_INCLUDED_ */