#ifndef _CW_CONFIG_H_INCLUDED_
#define _CW_CONFIG_H_INCLUDED_

#if (CW_LINUX)
#include <cw_linux_config.h>
#endif

#define cw_cdecl

typedef intptr_t cw_int_t;
typedef uintptr_t cw_uint_t;

#define CW_INT32_LEN   (sizeof("-2147483648") - 1)
#define CW_INT64_LEN   (sizeof("-9223372036854775808") - 1)

#define CW_INT_T_LEN   CW_INT32_LEN
#define CW_MAX_INT_T_VALUE  2147483647

#ifndef cw_inline
#define cw_inline      inline
#endif

#define cw_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define cw_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#ifndef CW_ALIGNMENT
#define CW_ALIGNMENT   sizeof(unsigned long)    /* platform word */
#endif

#endif /* _CW_CONFIG_H_INCLUDED_ */