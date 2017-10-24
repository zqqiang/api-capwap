#ifndef _CW_ARRAY_H_INCLUDED_
#define _CW_ARRAY_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

typedef struct {
    void        *elts;
    cw_uint_t   nelts;
    size_t       size;
    cw_uint_t   nalloc;
    cw_pool_t  *pool;
} cw_array_t;

#endif /* _CW_ARRAY_H_INCLUDED_ */