#ifndef _CW_FILE_H_INCLUDED_
#define _CW_FILE_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

struct cw_file_s {
    cw_fd_t                   fd;
    cw_str_t                  name;
    cw_file_info_t            info;

    off_t                      offset;
    off_t                      sys_offset;

    cw_log_t                 *log;

#if (CW_THREADS || CW_COMPAT)
    cw_int_t                (*thread_handler)(cw_thread_task_t *task,
                                               cw_file_t *file);
    void                      *thread_ctx;
    cw_thread_task_t         *thread_task;
#endif

#if (CW_HAVE_FILE_AIO || CW_COMPAT)
    cw_event_aio_t           *aio;
#endif

    unsigned                   valid_info:1;
    unsigned                   directio:1;
};

#endif /* _CW_FILE_H_INCLUDED_ */
