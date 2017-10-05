#ifndef _CW_CORE_H_INCLUDED_
#define _CW_CORE_H_INCLUDED_

#include <cw_config.h>

typedef struct cw_cycle_s cw_cycle_t;
typedef struct cw_log_s cw_log_t;

#define  CW_OK          0
#define  CW_ERROR      -1
#define  CW_AGAIN      -2
#define  CW_BUSY       -3
#define  CW_DONE       -4
#define  CW_DECLINED   -5
#define  CW_ABORT      -6

#include <cw_errno.h>
#include <cw_thread.h>
#include <cw_string.h>
#include <cw_process.h>
#include <cw_process_cycle.h>
#include <cw_cycle.h>
#include <cw_rbtree.h>
#include <cw_time.h>

#define cw_abs(value)       (((value) >= 0) ? (value) : - (value))
#define cw_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define cw_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

#endif /* _CW_CORE_H_INCLUDED_ */