#include <cw_config.h>
#include <cw_core.h>

void *
cw_alloc(size_t size, cw_log_t *log)
{
    void  *p;

    p = malloc(size);
    if (p == NULL) {
        cw_log_error(log, "malloc(%ld) failed", size);
    }

    cw_log_debug(log, "malloc: %p:%ld", p, size);

    return p;
}

void *
cw_memalign(size_t alignment, size_t size, cw_log_t *log)
{
    void  *p;
    int    err;

    err = posix_memalign(&p, alignment, size);

    if (err) {
        cw_log_error(log, "posix_memalign(%ld, %ld) failed", alignment, size);
        p = NULL;
    }

    cw_log_debug(log, "posix_memalign: %p:%ld @%ld", p, size, alignment);

    return p;
}