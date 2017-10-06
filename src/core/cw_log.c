#include <cw_config.h>
#include <cw_core.h>

static cw_log_t cw_log;

cw_log_t *
cw_log_init()
{
    if (zlog_init("./conf/zlog.conf")) {
        printf("zlog init failed\n");
        return NULL;
    }

    cw_log.c = zlog_get_category("master");
    if (!(cw_log.c)) {
        printf("get master cat fail\n");
        zlog_fini();
        return NULL;
    }

    return &cw_log;
}

void
cw_log_fini()
{
    zlog_fini();
}