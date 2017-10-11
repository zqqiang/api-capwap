
static cw_event_module_t  cw_epoll_module_ctx = {
    &epoll_name,
    cw_epoll_create_conf,               /* create configuration */
    cw_epoll_init_conf,                 /* init configuration */

    {
        cw_epoll_add_event,             /* add an event */
        cw_epoll_del_event,             /* delete an event */
        cw_epoll_add_event,             /* enable an event */
        cw_epoll_del_event,             /* disable an event */
        cw_epoll_add_connection,        /* add an connection */
        cw_epoll_del_connection,        /* delete an connection */
        cw_epoll_notify,                /* trigger a notify */
        cw_epoll_process_events,        /* process the events */
        cw_epoll_init,                  /* init the events */
        cw_epoll_done,                  /* done the events */
    }
};

cw_module_t  cw_epoll_module = {
    CW_MODULE_V1,
    &cw_epoll_module_ctx,               /* module context */
    cw_epoll_commands,                  /* module directives */
    CW_EVENT_MODULE,                    /* module type */
    NULL,                                /* init master */
    NULL,                                /* init module */
    NULL,                                /* init process */
    NULL,                                /* init thread */
    NULL,                                /* exit thread */
    NULL,                                /* exit process */
    NULL,                                /* exit master */
    CW_MODULE_V1_PADDING
};

static cw_int_t
cw_epoll_init(cw_cycle_t *cycle, cw_msec_t timer)
{
    cw_event_actions = cw_epoll_module_ctx.actions;

    return CW_OK;
}