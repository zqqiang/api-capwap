#include <cw_config.h>
#include <cw_core.h>

static cw_inline void *
cw_palloc_small(cw_pool_t *pool, size_t size, cw_uint_t align);
static void *cw_palloc_block(cw_pool_t *pool, size_t size);
static void *cw_palloc_large(cw_pool_t *pool, size_t size);

void *
cw_palloc(cw_pool_t *pool, size_t size)
{
    if (size <= pool->max) {
        return cw_palloc_small(pool, size, 1);
    }
    return cw_palloc_large(pool, size);
}

static void *
cw_palloc_block(cw_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    cw_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = cw_memalign(CW_POOL_ALIGNMENT, psize, pool->log);
    if (m == NULL) {
        return NULL;
    }

    new = (cw_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(cw_pool_data_t);
    m = cw_align_ptr(m, CW_ALIGNMENT);
    new->d.last = m + size;

    for (p = pool->current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    return m;
}

static cw_inline void *
cw_palloc_small(cw_pool_t *pool, size_t size, cw_uint_t align)
{
    u_char      *m;
    cw_pool_t  *p;

    p = pool->current;

    do {
        m = p->d.last;

        if (align) {
            m = cw_align_ptr(m, CW_ALIGNMENT);
        }

        if ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            return m;
        }

        p = p->d.next;

    } while (p);

    return cw_palloc_block(pool, size);
}

void *
cw_pcalloc(cw_pool_t *pool, size_t size)
{
    void *p;

    p = cw_palloc(pool, size);
    if (p) {
        cw_memzero(p, size);
    }

    return p;
}

static void *
cw_palloc_large(cw_pool_t *pool, size_t size)
{
    void              *p;
    cw_uint_t         n;
    cw_pool_large_t  *large;

    p = cw_alloc(size, pool->log);
    if (p == NULL) {
        return NULL;
    }

    n = 0;

    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

    large = cw_palloc_small(pool, sizeof(cw_pool_large_t), 1);
    if (large == NULL) {
        cw_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}