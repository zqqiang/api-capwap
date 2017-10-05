#ifndef _CW_THREAD_H_INCLUDED_
#define _CW_THREAD_H_INCLUDED_

typedef pid_t      cw_tid_t;
#define CW_TID_T_FMT         "%P"

cw_tid_t cw_thread_tid(void);
#define cw_log_tid           cw_thread_tid()

#endif /* _CW_THREAD_H_INCLUDED_ */