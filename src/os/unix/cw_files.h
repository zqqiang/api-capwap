#ifndef _CW_FILES_H_INCLUDED_
#define _CW_FILES_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

#define CW_INVALID_FILE         -1
#define CW_FILE_ERROR           -1


#define cw_open_file(name, mode, create, access)                            \
    open((const char *) name, mode|create, access)


#define CW_FILE_RDONLY          O_RDONLY
#define CW_FILE_OPEN            0

typedef int cw_fd_t;

#endif /* _CW_FILES_H_INCLUDED_ */