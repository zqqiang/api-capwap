#include <cw_config.h>
#include <cw_core.h>

ssize_t
cw_read_file(cw_file_t *file, u_char *buf, size_t size, off_t offset)
{
    ssize_t  n;

#if (CW_LINUX)
    cw_log_debug(file->log, "read: %d, %p, %ld, %ld", file->fd, buf, size, offset);
#else
    cw_log_debug(file->log, "read: %d, %p, %ld, %lld", file->fd, buf, size, offset);
#endif

#if (CW_HAVE_PREAD)

    n = pread(file->fd, buf, size, offset);

    if (n == -1) {
        cw_log_error(file->log, "pread() \"%s\" failed", file->name.data);
        return CW_ERROR;
    }

#else

    if (file->sys_offset != offset) {
        if (lseek(file->fd, offset, SEEK_SET) == -1) {
            cw_log_error(file->log, "lseek() \"%s\" failed", file->name.data);
            return CW_ERROR;
        }

        file->sys_offset = offset;
    }

    n = read(file->fd, buf, size);

    if (n == -1) {
        cw_log_error(file->log, "read() \"%s\" failed", file->name.data);
        return CW_ERROR;
    }

    file->sys_offset += n;

#endif

    file->offset += n;

    return n;
}
