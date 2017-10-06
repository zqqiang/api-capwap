#ifndef _CW_PROCESS_H_INCLUDED_
#define _CW_PROCESS_H_INCLUDED_

typedef pid_t cw_pid_t;

typedef void (*cw_spawn_proc_pt) (cw_cycle_t *cycle, void *data);

#define cw_getpid getpid

#ifndef cw_log_pid
#define cw_log_pid  cw_pid
#endif

cw_pid_t cw_spawn_process(cw_cycle_t *cycle,
    cw_spawn_proc_pt proc, void *data, char *name, cw_int_t respawn);

#define CW_MAX_PROCESSES         1024

#define CW_PROCESS_NORESPAWN     -1
#define CW_PROCESS_JUST_SPAWN    -2
#define CW_PROCESS_RESPAWN       -3
#define CW_PROCESS_JUST_RESPAWN  -4
#define CW_PROCESS_DETACHED      -5

cw_int_t cw_init_signals(cw_log_t *log);

extern cw_pid_t cw_pid;

#endif /* _CW_PROCESS_H_INCLUDED_ */