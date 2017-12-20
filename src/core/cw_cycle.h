#ifndef _CW_CYCLE_H_INCLUDED_
#define _CW_CYCLE_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

#ifndef CW_CYCLE_POOL_SIZE
#define CW_CYCLE_POOL_SIZE     CW_DEFAULT_POOL_SIZE
#endif

struct cw_cycle_s {
    void                  ****conf_ctx;
    cw_pool_t *pool;

    cw_log_t *log;

    cw_module_t **modules;
    cw_uint_t modules_n;

    cw_array_t listening;

    cw_str_t                 conf_file;
    cw_str_t                 conf_param;
};

typedef struct {
    cw_flag_t                daemon;
    cw_flag_t                master;

    cw_msec_t                timer_resolution;
    cw_msec_t                shutdown_timeout;

    cw_int_t                 worker_processes;
    cw_int_t                 debug_points;

    cw_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    int                       priority;

    // cw_uint_t                cpu_affinity_auto;
    // cw_uint_t                cpu_affinity_n;
    // cw_cpuset_t             *cpu_affinity;

    // char                     *username;
    // cw_uid_t                 user;
    // cw_gid_t                 group;

    cw_str_t                 working_directory;
    cw_str_t                 lock_file;

    cw_str_t                 pid;
    cw_str_t                 oldpid;

    cw_array_t               env;
    char                    **environment;
} cw_core_conf_t;

cw_cycle_t *cw_init_cycle(cw_cycle_t *old_cycle);

extern volatile cw_cycle_t  *cw_cycle;
extern cw_uint_t             cw_dump_config;

#endif /* _CW_CYCLE_H_INCLUDED_ */