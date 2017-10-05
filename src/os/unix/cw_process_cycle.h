#ifndef _CW_PROCESS_CYCLE_H_INCLUDED_
#define _CW_PROCESS_CYCLE_H_INCLUDED_

void cw_master_process_cycle(cw_cycle_t *cycle);

extern sig_atomic_t cw_quit;
extern sig_atomic_t cw_terminate;

#endif /* _CW_PROCESS_CYCLE_H_INCLUDED_ */