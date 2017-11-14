#ifndef _CW_CORE_H_INCLUDED_
#define _CW_CORE_H_INCLUDED_

#include <cw_config.h>

typedef struct cw_module_s cw_module_t;
typedef struct cw_conf_s cw_conf_t;
typedef struct cw_cycle_s cw_cycle_t;
typedef struct cw_log_s cw_log_t;
typedef struct cw_pool_s cw_pool_t;
typedef struct cw_chain_s cw_chain_t;
typedef struct cw_command_s cw_command_t;
typedef struct cw_file_s cw_file_t;
typedef struct cw_event_s cw_event_t;
typedef struct cw_connection_s cw_connection_t;

typedef void (*cw_event_handler_pt)(cw_event_t *ev);
typedef void (*cw_connection_handler_pt)(cw_connection_t *c);

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
#include <cw_files.h>
#include <cw_socket.h>
#include <cw_process.h>
#include <cw_process_cycle.h>
#include <cw_rbtree.h>
#include <cw_time.h>
#include <cw_log.h>
#include <cw_file.h>
#include <cw_channel.h>
#include <cw_module.h>
#include <cw_connection.h>
#include <cw_alloc.h>
#include <cw_palloc.h>
#include <cw_buf.h>
#include <cw_array.h>
#include <cw_cycle.h>
#include <cw_conf_file.h>

#define cw_abs(value)       (((value) >= 0) ? (value) : - (value))
#define cw_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define cw_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

#endif /* _CW_CORE_H_INCLUDED_ */