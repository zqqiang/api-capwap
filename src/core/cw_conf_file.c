#include <cw_config.h>
#include <cw_core.h>

#define CW_CONF_BUFFER  4096

static cw_int_t cw_conf_read_token(cw_conf_t *cf);
static cw_int_t cw_conf_handler(cw_conf_t *cf, cw_int_t last);

/* The eight fixed arguments */

static cw_uint_t argument_number[] = {
    CW_CONF_NOARGS,
    CW_CONF_TAKE1,
    CW_CONF_TAKE2,
    CW_CONF_TAKE3,
    CW_CONF_TAKE4,
    CW_CONF_TAKE5,
    CW_CONF_TAKE6,
    CW_CONF_TAKE7
};

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
            cw_log_error(cf->log, cw_open_file_n " \"%s\" failed", filename->data);
            return CW_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        if (cw_fd_info(fd, &cf->conf_file->file.info) == CW_FILE_ERROR) {
            cw_log_error(cf->log, cw_fd_info_n " \"%s\" failed", filename->data);
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
            // if (cw_conf_add_dump(cf, filename) != CW_OK) {
            //     goto failed;
            // }

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
                cw_log_error(cf->log, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        if (rc == CW_CONF_FILE_DONE) {

            if (type == parse_block) {
                cw_log_error(cf->log, "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        if (rc == CW_CONF_BLOCK_START) {

            if (type == parse_param) {
                cw_log_error(cf->log, "block directives are not supported ""in -g option");
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
                cw_log_error(cf->log, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            if (rv == CW_CONF_OK) {
                continue;
            }

            if (rv == CW_CONF_ERROR) {
                goto failed;
            }

            cw_log_error(cf->log, "%s", rv);

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

        if (cw_close_file(fd) == CW_FILE_ERROR) {
            cw_log_error(cf->log, cw_close_file_n " %s failed", filename->data);
            rc = CW_ERROR;
        }

        cf->conf_file = prev;
    }

    if (rc == CW_ERROR) {
        return CW_CONF_ERROR;
    }

    return CW_CONF_OK;
}

static cw_int_t
cw_conf_read_token(cw_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    cw_uint_t   found, need_space, last_space, sharp_comment, variable;
    cw_uint_t   quoted, s_quoted, d_quoted, start_line;
    cw_str_t   *word;
    cw_buf_t   *b, *dump;

    found = 0;
    need_space = 0;
    last_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->args->nelts = 0;
    b = cf->conf_file->buffer;
    dump = cf->conf_file->dump;
    start = b->pos;
    start_line = cf->conf_file->line;

    file_size = cw_file_size(&cf->conf_file->file.info);

    for ( ;; ) {

        if (b->pos >= b->last) {

            if (cf->conf_file->file.offset >= file_size) {

                if (cf->args->nelts > 0 || !last_space) {

                    if (cf->conf_file->file.fd == CW_INVALID_FILE) {
                        cw_log_error(cf->log, "unexpected end of parameter, ""expecting \";\"");
                        return CW_ERROR;
                    }

                    cw_log_error(cf->log, "unexpected end of file, ""expecting \";\" or \"}\"");
                    return CW_ERROR;
                }

                return CW_CONF_FILE_DONE;
            }

            len = b->pos - start;

            if (len == CW_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                if (d_quoted) {
                    ch = '"';

                } else if (s_quoted) {
                    ch = '\'';

                } else {
                    cw_log_error(cf->log, "too long parameter \"%*s...\" started",10, start);
                    return CW_ERROR;
                }

                cw_log_error(cf->log, "too long parameter, probably ""missing terminating \"%c\" character", ch);
                return CW_ERROR;
            }

            if (len) {
                cw_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            if (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = cw_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            if (n == CW_ERROR) {
                return CW_ERROR;
            }

            if (n != size) {
                cw_log_error(cf->log, cw_read_file_n" returned ""only %ld bytes instead of %ld", n, size);
                return CW_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            if (dump) {
                dump->last = cw_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        if (ch == LF) {
            cf->conf_file->line++;

            if (sharp_comment) {
                sharp_comment = 0;
            }
        }

        if (sharp_comment) {
            continue;
        }

        if (quoted) {
            quoted = 0;
            continue;
        }

        if (need_space) {
            if (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            if (ch == ';') {
                return CW_OK;
            }

            if (ch == '{') {
                return CW_CONF_BLOCK_START;
            }

            if (ch == ')') {
                last_space = 1;
                need_space = 0;

            } else {
                cw_log_error(cf->log, "unexpected \"%c\"", ch);
                return CW_ERROR;
            }
        }

        if (last_space) {
            if (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            switch (ch) {

            case ';':
            case '{':
                if (cf->args->nelts == 0) {
                    cw_log_error(cf->log, "unexpected \"%c\"", ch);
                    return CW_ERROR;
                }

                if (ch == '{') {
                    return CW_CONF_BLOCK_START;
                }

                return CW_OK;

            case '}':
                if (cf->args->nelts != 0) {
                    cw_log_error(cf->log, "unexpected \"}\"");
                    return CW_ERROR;
                }

                return CW_CONF_BLOCK_DONE;

            case '#':
                sharp_comment = 1;
                continue;

            case '\\':
                quoted = 1;
                last_space = 0;
                continue;

            case '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                continue;

            case '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                continue;

            default:
                last_space = 0;
            }

        } else {
            if (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            if (ch == '\\') {
                quoted = 1;
                continue;
            }

            if (ch == '$') {
                variable = 1;
                continue;
            }

            if (d_quoted) {
                if (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } else if (s_quoted) {
                if (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } else if (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            if (found) {
                word = cw_array_push(cf->args);
                if (word == NULL) {
                    return CW_ERROR;
                }

                word->data = cw_pnalloc(cf->pool, b->pos - 1 - start + 1);
                if (word->data == NULL) {
                    return CW_ERROR;
                }

                for (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    if (*src == '\\') {
                        switch (src[1]) {
                        case '"':
                        case '\'':
                        case '\\':
                            src++;
                            break;

                        case 't':
                            *dst++ = '\t';
                            src += 2;
                            continue;

                        case 'r':
                            *dst++ = '\r';
                            src += 2;
                            continue;

                        case 'n':
                            *dst++ = '\n';
                            src += 2;
                            continue;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                if (ch == ';') {
                    return CW_OK;
                }

                if (ch == '{') {
                    return CW_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}

static cw_int_t
cw_conf_handler(cw_conf_t *cf, cw_int_t last)
{
    char           *rv;
    void           *conf, **confp;
    cw_uint_t      i, found;
    cw_str_t      *name;
    cw_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    for (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        if (cmd == NULL) {
            continue;
        }

        for ( /* void */ ; cmd->name.len; cmd++) {

            if (name->len != cmd->name.len) {
                continue;
            }

            if (cw_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            if (cf->cycle->modules[i]->type != CW_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            if (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            if (!(cmd->type & CW_CONF_BLOCK) && last != CW_OK) {
                cw_log_error(cf->log, 
                             "directive \"%s\" is not terminated by \";\"",
                             name->data);
                return CW_ERROR;
            }

            if ((cmd->type & CW_CONF_BLOCK) && last != CW_CONF_BLOCK_START) {
                cw_log_error(cf->log,
                             "directive \"%s\" has no opening \"{\"",
                             name->data);
                return CW_ERROR;
            }

            /* is the directive's argument count right ? */

            if (!(cmd->type & CW_CONF_ANY)) {

                if (cmd->type & CW_CONF_FLAG) {

                    if (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } else if (cmd->type & CW_CONF_1MORE) {

                    if (cf->args->nelts < 2) {
                        goto invalid;
                    }

                } else if (cmd->type & CW_CONF_2MORE) {

                    if (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } else if (cf->args->nelts > CW_CONF_MAX_ARGS) {

                    goto invalid;

                } else if (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            if (cmd->type & CW_DIRECT_CONF) {
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } else if (cmd->type & CW_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } else if (cf->ctx) {
                confp = *(void **) ((char *) cf->ctx + cmd->conf);

                if (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            if (rv == CW_CONF_OK) {
                return CW_OK;
            }

            if (rv == CW_CONF_ERROR) {
                return CW_ERROR;
            }

            cw_log_error(cf->log, "\"%s\" directive %s", name->data, rv);

            return CW_ERROR;
        }
    }

    if (found) {
        cw_log_error(cf->log, "\"%s\" directive is not allowed here", name->data);

        return CW_ERROR;
    }

    cw_log_error(cf->log, "unknown directive \"%s\"", name->data);

    return CW_ERROR;

invalid:

    cw_log_error(cf->log, "invalid number of arguments in \"%s\" directive", name->data);

    return CW_ERROR;
}