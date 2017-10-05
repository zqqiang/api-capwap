#include <cw_config.h>
#include <cw_core.h>

cw_int_t
cw_log_init(zlog_category_t **c)
{
    if (zlog_init("./conf/zlog.conf")) {
        printf("zlog init failed\n");
        return CW_ERROR;
    }

    *c = zlog_get_category("master");
    if (!*c) {
        printf("get master cat fail\n");
        zlog_fini();
        return CW_ERROR;
    }

    return CW_OK;
}