#include <cw_config.h>
#include <cw_core.h>

char *
cw_conf_param(cw_conf_t *cf)
{
    char             *rv = NULL;
    cw_str_t        *param;
    cw_buf_t         b;
    cw_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    if (param->len == 0) {
        return CW_CONF_OK;
    }

    cw_memzero(&conf_file, sizeof(cw_conf_file_t));

    cw_memzero(&b, sizeof(cw_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = CW_INVALID_FILE;
    conf_file.file.name.data = NULL;
    conf_file.line = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    // rv = cw_conf_parse(cf, NULL);

    cf->conf_file = NULL;

    return rv;
}