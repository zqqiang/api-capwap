#ifndef _CW_CYCLE_H_INCLUDED_
#define _CW_CYCLE_H_INCLUDED_

#include <cw_config.h>
#include <cw_core.h>

struct cw_cycle_s {

    cw_log_t *log;

};

cw_cycle_t *cw_init_cycle(cw_cycle_t *old_cycle);

extern volatile cw_cycle_t  *cw_cycle;

#endif /* _CW_CYCLE_H_INCLUDED_ */