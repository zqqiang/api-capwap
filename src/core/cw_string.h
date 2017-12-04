#ifndef _CW_STRING_H_INCLUDED_
#define _CW_STRING_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

typedef struct {
    size_t      len;
    u_char     *data;
} cw_str_t;



#define cw_string(str)     { sizeof(str) - 1, (u_char *) str }
#define cw_null_string     { 0, NULL }
#define cw_str_set(str, text)                                               \
    (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text

#define cw_memzero(buf, n)       (void) memset(buf, 0, n)
#define cw_memset(buf, c, n)     (void) memset(buf, c, n)

#define cw_memcpy(dst, src, n)   (void) memcpy(dst, src, n)
#define cw_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))

#define cw_memmove(dst, src, n)   (void) memmove(dst, src, n)
#define cw_movemem(dst, src, n)   (((u_char *) memmove(dst, src, n)) + (n))

#define cw_strlen(s)       strlen((const char *) s)

#define cw_string_format(str) ((int)str.len), (str.data)

u_char *cw_cpystrn(u_char *dst, u_char *src, size_t n);

#endif /* _CW_STRING_H_INCLUDED_ */