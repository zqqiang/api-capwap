#ifndef _CW_CONF_FILE_H_INCLUDED_
#define _CW_CONF_FILE_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

struct cw_command_s {
    cw_str_t             name;
    cw_uint_t            type;
    char               *(*set)(cw_conf_t *cf, cw_command_t *cmd, void *conf);
    cw_uint_t            conf;
    cw_uint_t            offset;
    void                 *post;
};

#endif /* _CW_CONF_FILE_H_INCLUDED_ */