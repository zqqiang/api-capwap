#ifndef _CW_PALLOC_H_INCLUDED_
#define _CW_PALLOC_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

#define CW_POOL_ALIGNMENT       16

typedef struct cw_pool_large_s  cw_pool_large_t;

struct cw_pool_large_s {
    cw_pool_large_t     *next;
    void                 *alloc;
};

typedef struct {
    u_char               *last;
    u_char               *end;
    cw_pool_t           *next;
    cw_uint_t            failed;
} cw_pool_data_t;

struct cw_pool_s {
    cw_pool_data_t       d;
    size_t                max;
    cw_pool_t           *current;
    // cw_chain_t          *chain;
    cw_pool_large_t     *large;
    // cw_pool_cleanup_t   *cleanup;
    cw_log_t            *log;
};

void *cw_pcalloc(cw_pool_t *pool, size_t size);

#endif /* _CW_PALLOC_H_INCLUDED_ */