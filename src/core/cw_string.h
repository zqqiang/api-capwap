#ifndef _CW_STRING_H_INCLUDED_
#define _CW_STRING_H_INCLUDED_

#include <cw_config.h>

typedef struct {
    size_t      len;
    u_char     *data;
} cw_str_t;

#define cw_string(str)     { sizeof(str) - 1, (u_char *) str }
#define cw_null_string     { 0, NULL }

#define cw_memzero(buf, n)       (void) memset(buf, 0, n)
#define cw_memset(buf, c, n)     (void) memset(buf, c, n)

#define cw_memcpy(dst, src, n)   (void) memcpy(dst, src, n)
#define cw_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))

u_char * cw_cdecl cw_slprintf(u_char *buf, u_char *last, const char *fmt, ...);
u_char *cw_vslprintf(u_char *buf, u_char *last, const char *fmt, va_list args);

#endif /* _CW_STRING_H_INCLUDED_ */