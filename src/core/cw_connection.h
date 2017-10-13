#ifndef _CW_CONNECTION_H_INCLUDED_
#define _CW_CONNECTION_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

struct ngx_connection_s {

};

cw_int_t cw_open_listening_sockets(cw_cycle_t *cycle);

#endif /* _CW_CONNECTION_H_INCLUDED_ */