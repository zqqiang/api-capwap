#ifndef _CW_TIME_H_INCLUDED_
#define _CW_TIME_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

typedef cw_rbtree_key_t      cw_msec_t;
typedef cw_rbtree_key_int_t  cw_msec_int_t;

#define cw_msleep(ms)        (void) usleep(ms * 1000)

#endif /* _CW_TIME_H_INCLUDED_ */