/*
 * Copyright (C) Bill Qiang
 * Copyright (C) Nietsnie, Inc.
 */


#ifndef _CW_OS_H_INCLUDED_
#define _CW_OS_H_INCLUDED_


#include <cw_config.h>
#include <cw_core.h>


cw_int_t cw_os_init(cw_log_t *log);
cw_int_t cw_os_specific_init(cw_log_t *log);

extern cw_int_t    cw_ncpu;

#endif /* _CW_OS_H_INCLUDED_ */