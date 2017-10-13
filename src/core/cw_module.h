#ifndef _CW_MODULE_H_INCLUDED_
#define _CW_MODULE_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

struct cw_module_s {
    cw_uint_t            ctx_index;
    cw_uint_t            index;

    char                 *name;

    cw_uint_t            spare0;
    cw_uint_t            spare1;

    cw_uint_t            version;
    const char           *signature;

    void                 *ctx;
    cw_command_t        *commands;
    cw_uint_t            type;

    cw_int_t           (*init_master)(cw_log_t *log);

    cw_int_t           (*init_module)(cw_cycle_t *cycle);

    cw_int_t           (*init_process)(cw_cycle_t *cycle);
    cw_int_t           (*init_thread)(cw_cycle_t *cycle);
    void                (*exit_thread)(cw_cycle_t *cycle);
    void                (*exit_process)(cw_cycle_t *cycle);

    void                (*exit_master)(cw_cycle_t *cycle);

    uintptr_t             spare_hook0;
    uintptr_t             spare_hook1;
    uintptr_t             spare_hook2;
    uintptr_t             spare_hook3;
    uintptr_t             spare_hook4;
    uintptr_t             spare_hook5;
    uintptr_t             spare_hook6;
    uintptr_t             spare_hook7;
};

cw_int_t cw_preinit_modules(void);
cw_int_t cw_cycle_modules(cw_cycle_t *cycle);

extern cw_module_t *cw_modules[];
extern char *cw_module_names[];

#endif /* _CW_MODULE_H_INCLUDED_ */
