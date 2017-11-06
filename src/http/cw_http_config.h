#ifndef _CW_HTTP_CONFIG_H_INCLUDED_
#define _CW_HTTP_CONFIG_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>
#include <cw_http.h>

typedef struct {
    void        **main_conf;
    void        **srv_conf;
    void        **loc_conf;
} cw_http_conf_ctx_t;


#endif /* _CW_HTTP_CONFIG_H_INCLUDED_ */