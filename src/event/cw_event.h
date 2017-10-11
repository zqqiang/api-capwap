#ifndef _CW_EVENT_H_INCLUDED_
#define _CW_EVENT_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

typedef struct {
    cw_int_t  (*add)(cw_event_t *ev, cw_int_t event, cw_uint_t flags);
    cw_int_t  (*del)(cw_event_t *ev, cw_int_t event, cw_uint_t flags);

    cw_int_t  (*enable)(cw_event_t *ev, cw_int_t event, cw_uint_t flags);
    cw_int_t  (*disable)(cw_event_t *ev, cw_int_t event, cw_uint_t flags);

    cw_int_t  (*add_conn)(cw_connection_t *c);
    cw_int_t  (*del_conn)(cw_connection_t *c, cw_uint_t flags);

    cw_int_t  (*notify)(cw_event_handler_pt handler);

    cw_int_t  (*process_events)(cw_cycle_t *cycle, cw_msec_t timer, cw_uint_t flags);

    cw_int_t  (*init)(cw_cycle_t *cycle, cw_msec_t timer);
    void       (*done)(cw_cycle_t *cycle);
} cw_event_actions_t;

extern cw_event_actions_t   cw_event_actions;

#define cw_process_events   cw_event_actions.process_events

void cw_process_events_and_timers(cw_cycle_t *cycle);

#endif /* _CW_EVENT_H_INCLUDED_ */