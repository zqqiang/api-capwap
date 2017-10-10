#ifndef _CW_PROCESS_H_INCLUDED_
#define _CW_PROCESS_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

typedef pid_t cw_pid_t;

#define CW_INVALID_PID  -1

typedef void (*cw_spawn_proc_pt) (cw_cycle_t *cycle, void *data);

typedef struct {
    cw_pid_t            pid;
    int                 status;
    cw_socket_t         channel[2];

    cw_spawn_proc_pt    proc;
    void               *data;
    char               *name;

    unsigned            respawn:1;
    unsigned            just_spawn:1;
    unsigned            detached:1;
    unsigned            exiting:1;
    unsigned            exited:1;
} cw_process_t;

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