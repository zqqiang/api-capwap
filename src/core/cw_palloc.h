#ifndef _CW_PALLOC_H_INCLUDED_
#define _CW_PALLOC_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

struct cw_pool_s {
    // cw_pool_data_t       d;
    size_t                max;
    // cw_pool_t           *current;
    // cw_chain_t          *chain;
    // cw_pool_large_t     *large;
    // cw_pool_cleanup_t   *cleanup;
    cw_log_t            *log;
};

void *cw_pcalloc(cw_pool_t *pool, size_t size);

#endif /* _CW_PALLOC_H_INCLUDED_ */