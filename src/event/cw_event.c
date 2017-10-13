#include <cw_config.h>
#include <cw_core.h>
#include <cw_event.h>

extern cw_module_t cw_epoll_module;

cw_event_actions_t   cw_event_actions;

void
cw_process_events_and_timers(cw_cycle_t *cycle)
{
    cw_uint_t  flags = {0};
    cw_msec_t  timer = {0};

    (void) cw_process_events(cycle, timer, flags);
}