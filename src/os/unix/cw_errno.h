#ifndef _CW_ERROR_H_INCLUDED_
#define _CW_ERROR_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

typedef int cw_err_t;

#define CW_EADDRINUSE    EADDRINUSE

#define cw_socket_errno           errno

#endif /* _CW_ERROR_H_INCLUDED_ */