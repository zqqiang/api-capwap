#include <cw_config.h>
#include <cw_core.h>

typedef struct {
    int     signo;
    char   *signame;
    char   *name;
    void  (*handler)(int signo, siginfo_t *siginfo, void *ucontext);
} cw_signal_t;

static void cw_signal_handler(int signo, siginfo_t *siginfo, void *ucontext);

cw_signal_t  signals[] = {
    { SIGQUIT, "SIGQUIT", "quit", cw_signal_handler },
    { SIGTERM, "SIGTERM", "stop", cw_signal_handler },
};

cw_int_t
cw_init_signals(cw_log_t *log)
{
    cw_signal_t *sig;
    struct sigaction sa;

    for (sig = signals; sig->signo != 0; sig++) {
        cw_memzero(&sa, sizeof(struct sigaction));

        if (sig->handler) {
            sa.sa_sigaction = sig->handler;
            sa.sa_flags = SA_SIGINFO;
        } else {
            sa.sa_handler = SIG_IGN;
        }

        sigemptyset(&sa.sa_mask);

        if (sigaction(sig->signo, &sa, NULL) == -1) {
            cw_log_error(log, "sigaction(%s) failed", sig->signame);
            return CW_ERROR;
        }
    }

    return CW_OK;
}

static void
cw_signal_handler(int signo, siginfo_t *siginfo, void *ucontext)
{
    char *action = "";
    cw_signal_t *sig = NULL;

    for (sig = signals; sig->signo != 0; sig++) {
        if (sig->signo == signo) {
            break;
        }
    }

    switch(signo) {
        case SIGQUIT:
            cw_quit = 1;
            action = ", shutting down";
            break;
        case SIGTERM:
            cw_terminate = 2;
            action = ", exiting";
            break;
    }

    if (siginfo && siginfo->si_pid) {
        cw_log_notice(cw_cycle->log, "signal %d (%s) received from %d%s", signo, sig->signame, siginfo->si_pid, action);
    } else {
        cw_log_notice(cw_cycle->log, "signal %d (%s) received%s", signo, sig->signame, action);
    }
}