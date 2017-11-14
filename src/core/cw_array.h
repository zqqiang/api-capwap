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


cw_array_t *cw_array_create(cw_pool_t *p, cw_uint_t n, size_t size);
void cw_array_destroy(cw_array_t *a);
void *cw_array_push(cw_array_t *a);
void *cw_array_push_n(cw_array_t *a, cw_uint_t n);


static cw_inline cw_int_t
cw_array_init(cw_array_t *array, cw_pool_t *pool, cw_uint_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = cw_palloc(pool, n * size);
    if (array->elts == NULL) {
        return CW_ERROR;
    }

    return CW_OK;
}


#endif /* _CW_ARRAY_H_INCLUDED_ */
