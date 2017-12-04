#include <cw_config.h>
#include <cw_core.h>

extern cw_module_t  cw_core_module;
extern cw_module_t  cw_http_module;

cw_module_t *cw_modules[] = {
    &cw_core_module,
    &cw_http_module,
    NULL
};

char *cw_module_names[] = {
    "cw_core_module",
    "cw_http_module",
    NULL
};