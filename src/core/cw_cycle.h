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

    cw_int_t worker_processes;

} cw_core_conf_t;

cw_cycle_t *cw_init_cycle(cw_cycle_t *old_cycle);

extern volatile cw_cycle_t  *cw_cycle;

#endif /* _CW_CYCLE_H_INCLUDED_ */