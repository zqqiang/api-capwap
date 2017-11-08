#ifndef _CW_BUF_H_INCLUDED_
#define _CW_BUF_H_INCLUDED_


#include <cw_config.h>
#include <cw_core.h>

typedef void *            cw_buf_tag_t;

typedef struct cw_buf_s  cw_buf_t;

struct cw_buf_s {
    u_char          *pos;
    u_char          *last;
    off_t            file_pos;
    off_t            file_last;

    u_char          *start;         /* start of buffer */
    u_char          *end;           /* end of buffer */
    cw_buf_tag_t    tag;
    // cw_file_t      *file;
    cw_buf_t       *shadow;


    /* the buf's content could be changed */
    unsigned         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    unsigned         memory:1;

    /* the buf's content is mmap()ed and must not be changed */
    unsigned         mmap:1;

    unsigned         recycled:1;
    unsigned         in_file:1;
    unsigned         flush:1;
    unsigned         sync:1;
    unsigned         last_buf:1;
    unsigned         last_in_chain:1;

    unsigned         last_shadow:1;
    unsigned         temp_file:1;

    /* STUB */ int   num;
};

struct cw_chain_s {
    cw_buf_t    *buf;
    cw_chain_t  *next;
};

#endif /* _CW_BUF_H_INCLUDED_ */