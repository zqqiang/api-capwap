#ifndef _CW_SOCKET_H_INCLUDED_
#define _CW_SOCKET_H_INCLUDED_

#include <cw_config.h>

typedef int cw_socket_t;

int cw_nonblocking(cw_socket_t s);
int cw_blocking(cw_socket_t s);

#define cw_nonblocking_n   "ioctl(FIONBIO)"
#define cw_blocking_n      "ioctl(!FIONBIO)"

#endif /* _CW_SOCKET_H_INCLUDED_ */