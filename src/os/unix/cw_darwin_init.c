
/*
 * Copyright (C) Bill Qiang
 * Copyright (C) Nietsnie, Inc.
 */


#include <cw_config.h>
#include <cw_core.h>



cw_int_t
cw_os_specific_init(cw_log_t *log)
{
    size_t      size;
    cw_err_t   err;
    cw_uint_t  i;

    size = sizeof(cw_darwin_kern_ostype);
    if (sysctlbyname("kern.ostype", cw_darwin_kern_ostype, &size, NULL, 0)
        == -1)
    {
        err = cw_errno;

        if (err != CW_ENOENT) {

            cw_log_error(CW_LOG_ALERT, log, err,
                          "sysctlbyname(kern.ostype) failed");

            if (err != CW_ENOMEM) {
                return CW_ERROR;
            }

            cw_darwin_kern_ostype[size - 1] = '\0';
        }
    }

    size = sizeof(cw_darwin_kern_osrelease);
    if (sysctlbyname("kern.osrelease", cw_darwin_kern_osrelease, &size,
                     NULL, 0)
        == -1)
    {
        err = cw_errno;

        if (err != CW_ENOENT) {

            cw_log_error(CW_LOG_ALERT, log, err,
                          "sysctlbyname(kern.osrelease) failed");

            if (err != CW_ENOMEM) {
                return CW_ERROR;
            }

            cw_darwin_kern_osrelease[size - 1] = '\0';
        }
    }

    for (i = 0; sysctls[i].name; i++) {
        size = sysctls[i].size;

        if (sysctlbyname(sysctls[i].name, sysctls[i].value, &size, NULL, 0)
            == 0)
        {
            sysctls[i].exists = 1;
            continue;
        }

        err = cw_errno;

        if (err == CW_ENOENT) {
            continue;
        }

        cw_log_error(CW_LOG_ALERT, log, err,
                      "sysctlbyname(%s) failed", sysctls[i].name);
        return CW_ERROR;
    }

    cw_ncpu = cw_darwin_hw_ncpu;

    if (cw_darwin_kern_ipc_somaxconn > 32767) {
        cw_log_error(CW_LOG_ALERT, log, 0,
                      "sysctl kern.ipc.somaxconn must be less than 32768");
        return CW_ERROR;
    }

    cw_tcp_nodelay_and_tcp_nopush = 1;

    cw_os_io = cw_darwin_io;

    return CW_OK;
}