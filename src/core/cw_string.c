#include <cw_config.h>
#include <cw_core.h>

u_char *
cw_cpystrn(u_char *dst, u_char *src, size_t n)
{
    if (n == 0) {
        return dst;
    }

    while (--n) {
        *dst = *src;

        if (*dst == '\0') {
            return dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    return dst;
}

cw_int_t
cw_atoi(u_char *line, size_t n)
{
    cw_int_t  value, cutoff, cutlim;

    if (n == 0) {
        return CW_ERROR;
    }

    cutoff = CW_MAX_INT_T_VALUE / 10;
    cutlim = CW_MAX_INT_T_VALUE % 10;

    for (value = 0; n--; line++) {
        if (*line < '0' || *line > '9') {
            return CW_ERROR;
        }

        if (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            return CW_ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    return value;
}