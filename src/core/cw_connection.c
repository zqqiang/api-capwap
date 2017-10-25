#include <cw_config.h>
#include <cw_core.h>
#include <cw_event.h>

cw_int_t
cw_open_listening_sockets(cw_cycle_t *cycle)
{
    cw_err_t     err;
    int reuseaddr = 1;
    cw_uint_t i = 0;
    cw_socket_t s;
    cw_listening_t *ls = NULL;

    ls = cycle->listening.elts;

    for (i = 0; i < cycle->listening.nelts; ++i) {
        if (ls[i].ignore) {
            continue;
        }

        if (ls[i].fd != (cw_socket_t) -1) {
            continue;
        }

        s = cw_socket(ls[i].sockaddr->sa_family, ls[i].type, 0);

        if (s == (cw_socket_t) -1) {
            cw_log_error(cycle->log, cw_socket_n " %.*s failed", cw_string_format(ls[i].addr_text));
            return CW_ERROR;
        }

        if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                       (const void *) &reuseaddr, sizeof(int))
            == -1)
        {
            cw_log_error(cycle->log,
                          "setsockopt(SO_REUSEADDR) %.*s failed",
                          cw_string_format(ls[i].addr_text));

            if (cw_close_socket(s) == -1) {
                cw_log_error(cycle->log,
                              cw_close_socket_n " %.*s failed",
                              cw_string_format(ls[i].addr_text));
            }

            return CW_ERROR;
        }

        if (cw_nonblocking(s) == -1) {
            cw_log_error(cycle->log,
                          cw_nonblocking_n " %.*s failed",
                          cw_string_format(ls[i].addr_text));

            if (cw_close_socket(s) == -1) {
                cw_log_error(cycle->log,
                              cw_close_socket_n " %.*s failed",
                              cw_string_format(ls[i].addr_text));
            }

            return CW_ERROR;
        }

        cw_log_debug(cycle->log, "bind() %.*s #%d ", cw_string_format(ls[i].addr_text), s);

        if (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
            err = cw_socket_errno;

            if (err != CW_EADDRINUSE) {
                cw_log_error(cycle->log, "bind() to %.*s failed", cw_string_format(ls[i].addr_text));
            }

            if (cw_close_socket(s) == -1) {
                cw_log_error(cycle->log,
                              cw_close_socket_n " %.*s failed",
                              cw_string_format(ls[i].addr_text));
            }

            if (err != CW_EADDRINUSE) {
                return CW_ERROR;
            }

            continue;
        }

        if (ls[i].type != SOCK_STREAM) {
            ls[i].fd = s;
            continue;
        }

        if (listen(s, ls[i].backlog) == -1) {
            err = cw_socket_errno;

            if (err != CW_EADDRINUSE) {
                cw_log_error(cycle->log,
                              "listen() to %.*s, backlog %d failed",
                              cw_string_format(ls[i].addr_text), ls[i].backlog);
            }

            if (cw_close_socket(s) == -1) {
                cw_log_error(cycle->log,
                              cw_close_socket_n " %.*s failed",
                              cw_string_format(ls[i].addr_text));
            }

            if (err != CW_EADDRINUSE) {
                return CW_ERROR;
            }

            continue;
        }

        ls[i].listen = 1;
        ls[i].fd = s;
    }

    return CW_OK;
}