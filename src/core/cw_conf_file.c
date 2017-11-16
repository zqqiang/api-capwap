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

    rv = cw_conf_parse(cf, NULL);

    cf->conf_file = NULL;

    return rv;
}


char *
cw_conf_parse(cw_conf_t *cf, cw_str_t *filename)
{
    char             *rv;
    cw_fd_t          fd;
    cw_int_t         rc;
    cw_buf_t         buf;
    cw_conf_file_t  *prev, conf_file;
    enum {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#if (CW_SUPPRESS_WARN)
    fd = CW_INVALID_FILE;
    prev = NULL;
#endif

    if (filename) {

        /* open configuration file */

        fd = cw_open_file(filename->data, CW_FILE_RDONLY, CW_FILE_OPEN, 0);

        if (fd == CW_INVALID_FILE) {
            cw_conf_log_error(CW_LOG_EMERG, cf, cw_errno,
                               cw_open_file_n " \"%s\" failed",
                               filename->data);
            return CW_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        if (cw_fd_info(fd, &cf->conf_file->file.info) == cw_FILE_ERROR) {
            cw_log_error(CW_LOG_EMERG, cf->log, cw_errno,
                          cw_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = cw_alloc(CW_CONF_BUFFER, cf->log);
        if (buf.start == NULL) {
            goto failed;
        }

        buf.pos = buf.start;
        buf.last = buf.start;
        buf.end = buf.last + CW_CONF_BUFFER;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.data = filename->data;
        cf->conf_file->file.offset = 0;
        cf->conf_file->file.log = cf->log;
        cf->conf_file->line = 1;

        type = parse_file;

        if (cw_dump_config
#if (CW_DEBUG)
            || 1
#endif
           )
        {
            if (cw_conf_add_dump(cf, filename) != CW_OK) {
                goto failed;
            }

        } else {
            cf->conf_file->dump = NULL;
        }

    } else if (cf->conf_file->file.fd != CW_INVALID_FILE) {

        type = parse_block;

    } else {
        type = parse_param;
    }


    for ( ;; ) {
        rc = cw_conf_read_token(cf);

        /*
         * cw_conf_read_token() may return
         *
         *    CW_ERROR             there is error
         *    CW_OK                the token terminated by ";" was found
         *    CW_CONF_BLOCK_START  the token terminated by "{" was found
         *    CW_CONF_BLOCK_DONE   the "}" was found
         *    CW_CONF_FILE_DONE    the configuration file is done
         */

        if (rc == CW_ERROR) {
            goto done;
        }

        if (rc == CW_CONF_BLOCK_DONE) {

            if (type != parse_block) {
                cw_conf_log_error(CW_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        if (rc == CW_CONF_FILE_DONE) {

            if (type == parse_block) {
                cw_conf_log_error(CW_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        if (rc == CW_CONF_BLOCK_START) {

            if (type == parse_param) {
                cw_conf_log_error(cw_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == CW_OK || rc == CW_CONF_BLOCK_START */

        if (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            if (rc == CW_CONF_BLOCK_START) {
                cw_conf_log_error(cw_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            if (rv == CW_CONF_OK) {
                continue;
            }

            if (rv == CW_CONF_ERROR) {
                goto failed;
            }

            cw_conf_log_error(cw_LOG_EMERG, cf, 0, rv);

            goto failed;
        }


        rc = cw_conf_handler(cf, rc);

        if (rc == CW_ERROR) {
            goto failed;
        }
    }

failed:

    rc = CW_ERROR;

done:

    if (filename) {
        if (cf->conf_file->buffer->start) {
            cw_free(cf->conf_file->buffer->start);
        }

        if (cw_close_file(fd) == cw_FILE_ERROR) {
            cw_log_error(cw_LOG_ALERT, cf->log, cw_errno,
                          cw_close_file_n " %s failed",
                          filename->data);
            rc = CW_ERROR;
        }

        cf->conf_file = prev;
    }

    if (rc == CW_ERROR) {
        return CW_CONF_ERROR;
    }

    return CW_CONF_OK;
}
