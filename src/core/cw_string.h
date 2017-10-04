#ifndef _CW_STRING_H_INCLUDED_
#define _CW_STRING_H_INCLUDED_

#include <cw_config.h>

#define cw_memzero(buf, n)       (void) memset(buf, 0, n)
#define cw_memset(buf, c, n)     (void) memset(buf, c, n)

#endif /* _CW_STRING_H_INCLUDED_ */