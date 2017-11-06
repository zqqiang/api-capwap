#ifndef _CW_ALLOC_H_INCLUDED_
#define _CW_ALLOC_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

void *cw_alloc(size_t size, cw_log_t *log);
void *cw_calloc(size_t size, cw_log_t *log);

#define cw_free          free

void *cw_memalign(size_t alignment, size_t size, cw_log_t *log);

#endif /* _CW_ALLOC_H_INCLUDED_ */