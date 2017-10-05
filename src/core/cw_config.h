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


#endif /* _CW_CONFIG_H_INCLUDED_ */