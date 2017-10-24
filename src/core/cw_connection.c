#include <cw_config.h>
#include <cw_core.h>
#include <cw_event.h>

cw_int_t
cw_open_listening_sockets(cw_cycle_t *cycle)
{
    int reuseaddr = 1;
    cw_uint_t tries = 0;
    cw_uint_t i = 0;
    cw_socket_t s;
    cw_listening_t *ls = NULL;

    for (tries = 5; tries; tries--) {

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
                cw_log_error(cycle->log, cw_socket_n " %V failed", &ls[i].addr_text);
                return CW_ERROR;
            }

            if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
                           (const void *) &reuseaddr, sizeof(int))
                == -1)
            {
                ngx_log_error(cycle->log,
                              "setsockopt(SO_REUSEADDR) %V failed",
                              &ls[i].addr_text);

                if (cw_close_socket(s) == -1) {
                    cw_log_error(cycle->log,
                                  cw_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                return CW_ERROR;
            }

            if (!(cw_event_flags & CW_USE_IOCP_EVENT)) {
                if (cw_nonblocking(s) == -1) {
                    cw_log_error(cycle->log,
                                  cw_nonblocking_n " %V failed",
                                  &ls[i].addr_text);

                    if (cw_close_socket(s) == -1) {
                        cw_log_error(cycle->log,
                                      cw_close_socket_n " %V failed",
                                      &ls[i].addr_text);
                    }

                    return CW_ERROR;
                }
            }

            cw_log_debug(cycle->log, "bind() %V #%d ", &ls[i].addr_text, s);

            if (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {
                err = cw_socket_errno;

                if (err != CW_EADDRINUSE) {
                    cw_log_error(cycle->log, "bind() to %V failed", &ls[i].addr_text);
                }

                if (cw_close_socket(s) == -1) {
                    cw_log_error(cycle->log,
                                  cw_close_socket_n " %V failed",
                                  &ls[i].addr_text);
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
                                  "listen() to %V, backlog %d failed",
                                  &ls[i].addr_text, ls[i].backlog);
                }

                if (cw_close_socket(s) == -1) {
                    cw_log_error(cycle->log,
                                  cw_close_socket_n " %V failed",
                                  &ls[i].addr_text);
                }

                if (err != CW_EADDRINUSE) {
                    return CW_ERROR;
                }

                continue;
            }

            ls[i].listen = 1;
            ls[i].fd = s;
        }

        if (!failed) {
            break;
        }

        cw_log_notice(cycle->log,
                      "try again to bind() after 500ms");

        cw_msleep(500);
    }

    return CW_OK;
}