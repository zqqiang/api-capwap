#ifndef _CW_FILES_H_INCLUDED_
#define _CW_FILES_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

typedef int cw_fd_t;
typedef struct stat              cw_file_info_t;

#define CW_INVALID_FILE         -1
#define CW_FILE_ERROR           -1


#define cw_open_file(name, mode, create, access)                            \
    open((const char *) name, mode|create, access)


#define cw_open_file_n          "open()"

ssize_t cw_read_file(cw_file_t *file, u_char *buf, size_t size, off_t offset);
#if (CW_HAVE_PREAD)
#define cw_read_file_n          "pread()"
#else
#define cw_read_file_n          "read()"
#endif

#define CW_FILE_RDONLY          O_RDONLY
#define CW_FILE_OPEN            0

#define cw_close_file           close
#define cw_close_file_n         "close()"

#define cw_fd_info(fd, sb)      fstat(fd, sb)
#define cw_fd_info_n            "fstat()"

#define cw_file_size(sb)        (sb)->st_size

#endif /* _CW_FILES_H_INCLUDED_ */