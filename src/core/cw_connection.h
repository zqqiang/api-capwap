#ifndef _CW_CONNECTION_H_INCLUDED_
#define _CW_CONNECTION_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

typedef struct cw_listening_s  cw_listening_t;

struct cw_listening_s {
    cw_socket_t        fd;

    struct sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    size_t              addr_text_max_len;
    cw_str_t           addr_text;

    int                 type;

    int                 backlog;

    unsigned            open:1;
    unsigned            remain:1;
    unsigned            ignore:1;

    unsigned            bound:1;       /* already bound */
    unsigned            inherited:1;   /* inherited from previous process */
    unsigned            nonblocking_accept:1;
    unsigned            listen:1;
    unsigned            nonblocking:1;
    unsigned            shared:1;    /* shared between threads or processes */
    unsigned            addr_ntop:1;
    unsigned            wildcard:1;

    unsigned            reuseport:1;
    unsigned            add_reuseport:1;
    unsigned            keepalive:2;

    unsigned            deferred_accept:1;
    unsigned            delete_deferred:1;
    unsigned            add_deferred:1;
};

struct cw_connection_s {

};

cw_int_t cw_open_listening_sockets(cw_cycle_t *cycle);

#endif /* _CW_CONNECTION_H_INCLUDED_ */