#ifndef _CW_CONF_FILE_H_INCLUDED_
#define _CW_CONF_FILE_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

#define CW_CONF_OK          NULL
#define CW_CONF_ERROR       (void *) -1

#define CW_CORE_MODULE      0x45524F43  /* "CORE" */
#define CW_CONF_MODULE      0x464E4F43  /* "CONF" */

struct cw_command_s {
    cw_str_t             name;
    cw_uint_t            type;
    char               *(*set)(cw_conf_t *cf, cw_command_t *cmd, void *conf);
    cw_uint_t            conf;
    cw_uint_t            offset;
    void                 *post;
};

struct cw_conf_s {
    // char                 *name;
    // cw_array_t          *args;

    // cw_cycle_t          *cycle;
    // cw_pool_t           *pool;
    // cw_pool_t           *temp_pool;
    // cw_conf_file_t      *conf_file;
    // cw_log_t            *log;

    // void                 *ctx;
    // cw_uint_t            module_type;
    // cw_uint_t            cmd_type;

    // cw_conf_handler_pt   handler;
    // char                 *handler_conf;
};

#endif /* _CW_CONF_FILE_H_INCLUDED_ */