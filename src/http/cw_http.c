#include <cw_config.h>
#include <cw_core.h>
#include <cw_http.h>

static char *cw_http_block(cw_conf_t *cf, cw_command_t *cmd, void *conf);

static cw_command_t  cw_http_commands[] = {

    { cw_string("http"),
      CW_MAIN_CONF|CW_CONF_BLOCK|CW_CONF_NOARGS,
      cw_http_block,
      0,
      0,
      NULL },

      cw_null_command
};


static cw_core_module_t  cw_http_module_ctx = {
    cw_string("http"),
    NULL,
    NULL
};


cw_module_t  cw_http_module = {
    CW_MODULE_V1,
    &cw_http_module_ctx,                  /* module context */
    cw_http_commands,                     /* module directives */
    CW_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    CW_MODULE_V1_PADDING
};

static char *
cw_http_block(cw_conf_t *cf, cw_command_t *cmd, void *conf)
{
    cw_http_conf_ctx_t         *ctx;

    if (*(cw_http_conf_ctx_t**)conf) {
        return "is duplicate";
    }

    ctx = cw_pcalloc(cf->pool, sizeof(cw_http_conf_ctx_t));
    if (ctx == NULL) {
        return CW_CONF_ERROR;
    }

    *(cw_http_conf_ctx_t **) conf = ctx;

    return CW_CONF_OK;
}