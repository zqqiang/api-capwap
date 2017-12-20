#ifndef _CW_CONF_FILE_H_INCLUDED_
#define _CW_CONF_FILE_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#define CW_CONF_NOARGS      0x00000001
#define CW_CONF_TAKE1       0x00000002
#define CW_CONF_TAKE2       0x00000004
#define CW_CONF_TAKE3       0x00000008
#define CW_CONF_TAKE4       0x00000010
#define CW_CONF_TAKE5       0x00000020
#define CW_CONF_TAKE6       0x00000040
#define CW_CONF_TAKE7       0x00000080

#define CW_CONF_MAX_ARGS    8

#define CW_CONF_NOARGS      0x00000001

#define CW_CONF_BLOCK       0x00000100

#define CW_MAIN_CONF        0x01000000

#define CW_CONF_ARGS_NUMBER 0x000000ff
#define CW_CONF_BLOCK       0x00000100
#define CW_CONF_FLAG        0x00000200
#define CW_CONF_ANY         0x00000400
#define CW_CONF_1MORE       0x00000800
#define CW_CONF_2MORE       0x00001000

#define CW_DIRECT_CONF      0x00010000

#define CW_MAIN_CONF        0x01000000
#define CW_ANY_CONF         0x1F000000

#define CW_CONF_UNSET       -1
#define CW_CONF_UNSET_UINT  (cw_uint_t) -1
#define CW_CONF_UNSET_PTR   (void *) -1
#define CW_CONF_UNSET_SIZE  (size_t) -1
#define CW_CONF_UNSET_MSEC  (cw_msec_t) -1

#define CW_CONF_OK          NULL
#define CW_CONF_ERROR       (void *) -1

#define CW_CONF_BLOCK_START 1
#define CW_CONF_BLOCK_DONE  2
#define CW_CONF_FILE_DONE   3

#define CW_CORE_MODULE      0x45524F43  /* "CORE" */
#define CW_CONF_MODULE      0x464E4F43  /* "CONF" */

struct cw_command_s {
    cw_str_t             name;
    cw_uint_t            type;
    char               *(*set)(cw_conf_t *cf, cw_command_t *cmd, void *conf);
    cw_uint_t            conf;
    cw_uint_t            offset;
    void                 *post;
};

typedef struct {
    cw_file_t            file;
    cw_buf_t            *buffer;
    cw_buf_t            *dump;
    cw_uint_t            line;
} cw_conf_file_t;

#define cw_null_command  { cw_null_string, 0, NULL, 0, 0, NULL }

typedef char *(*cw_conf_handler_pt)(cw_conf_t *cf,
    cw_command_t *dummy, void *conf);

struct cw_conf_s {
    char                 *name;
    cw_array_t          *args;

    cw_cycle_t          *cycle;
    cw_pool_t           *pool;
    cw_pool_t           *temp_pool;
    cw_conf_file_t      *conf_file;
    cw_log_t            *log;

    void                 *ctx;
    cw_uint_t            module_type;
    cw_uint_t            cmd_type;

    cw_conf_handler_pt   handler;
    char                 *handler_conf;
};

char *cw_conf_param(cw_conf_t *cf);
char *cw_conf_parse(cw_conf_t *cf, cw_str_t *filename);

#endif /* _CW_CONF_FILE_H_INCLUDED_ */