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
cw_init_signals()
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
            printf("sigaction(%s) failed\n", sig->signame);
            return CW_ERROR;
        }
    }

    return CW_OK;
}

static void
cw_signal_handler(int signo, siginfo_t *siginfo, void *ucontext)
{
    printf("%s\n", __FUNCTION__);
}