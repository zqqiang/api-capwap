#ifndef _CW_SOCKET_H_INCLUDED_
#define _CW_SOCKET_H_INCLUDED_

#include <cw_config.h>

typedef int cw_socket_t;

#define cw_socket          socket
#define cw_socket_n        "socket()"

int cw_nonblocking(cw_socket_t s);
int cw_blocking(cw_socket_t s);

#define cw_nonblocking_n   "ioctl(FIONBIO)"
#define cw_blocking_n      "ioctl(!FIONBIO)"

#define cw_close_socket    close
#define cw_close_socket_n  "close() socket"

#endif /* _CW_SOCKET_H_INCLUDED_ */