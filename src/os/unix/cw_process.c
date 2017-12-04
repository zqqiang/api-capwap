#include <cw_config.h>
#include <cw_core.h>

typedef struct {
    int     signo;
    char   *signame;
    char   *name;
    void  (*handler)(int signo, siginfo_t *siginfo, void *ucontext);
} cw_signal_t;

static void cw_signal_handler(int signo, siginfo_t *siginfo, void *ucontext);

cw_int_t cw_process_slot;
cw_socket_t cw_channel;
cw_int_t cw_last_process;
cw_process_t cw_processes[CW_MAX_PROCESSES];

cw_signal_t  signals[] = {
    { SIGQUIT, "SIGQUIT", "quit", cw_signal_handler },
    { SIGTERM, "SIGTERM", "stop", cw_signal_handler },
    { 0, NULL, "", NULL }
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

cw_pid_t
cw_spawn_process(cw_cycle_t *cycle, cw_spawn_proc_pt proc, void *data,
    char *name, cw_int_t respawn)
{
    u_long on;
    cw_pid_t pid = {0};
    cw_int_t s = 0;

    if (respawn >= 0) {
        s = respawn;

    } else {
        for (s = 0; s < cw_last_process; s++) {
            if (cw_processes[s].pid == -1) {
                break;
            }
        }

        if (s == CW_MAX_PROCESSES) {
            cw_log_fatal(cycle->log, "no more than %d processes can be spawned", CW_MAX_PROCESSES);
            return CW_INVALID_PID;
        }
    }

    if (respawn != CW_PROCESS_DETACHED) {
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, cw_processes[s].channel) == -1) {
            cw_log_fatal(cycle->log, "socketpair() failed while spawning \"%s\"", name);
            return CW_INVALID_PID;
        }

        cw_log_debug(cycle->log, "channel %d:%d", cw_processes[s].channel[0], cw_processes[s].channel[1]);

        if (cw_nonblocking(cw_processes[s].channel[0]) == -1) {
            cw_log_fatal(cycle->log, cw_nonblocking_n " failed while spawning \"%s\"", name);
            cw_close_channel(cw_processes[s].channel, cycle->log);
            return CW_INVALID_PID;
        }
        if (cw_nonblocking(cw_processes[s].channel[1]) == -1) {
            cw_log_fatal(cycle->log, cw_nonblocking_n " failed while spawning \"%s\"", name);
            cw_close_channel(cw_processes[s].channel, cycle->log);
            return CW_INVALID_PID;
        }

        on = 1;
        if (ioctl(cw_processes[s].channel[0], FIOASYNC, &on) == -1) {
            cw_log_fatal(cycle->log, "ioctl(FIOASYNC) failed while spawning \"%s\"", name);
            cw_close_channel(cw_processes[s].channel, cycle->log);
            return CW_INVALID_PID;
        }

        if (fcntl(cw_processes[s].channel[0], F_SETOWN, cw_pid) == -1) {
            cw_log_fatal(cycle->log, "fcntl(F_SETOWN) failed while spawning \"%s\"", name);
            cw_close_channel(cw_processes[s].channel, cycle->log);
            return CW_INVALID_PID;
        }

        if (fcntl(cw_processes[s].channel[0], F_SETFD, FD_CLOEXEC) == -1) {
            cw_log_fatal(cycle->log, "fcntl(FD_CLOEXEC) failed while spawning \"%s\"",
                           name);
            cw_close_channel(cw_processes[s].channel, cycle->log);
            return CW_INVALID_PID;
        }

        if (fcntl(cw_processes[s].channel[1], F_SETFD, FD_CLOEXEC) == -1) {
            cw_log_fatal(cycle->log, "fcntl(FD_CLOEXEC) failed while spawning \"%s\"",
                           name);
            cw_close_channel(cw_processes[s].channel, cycle->log);
            return CW_INVALID_PID;
        }

        cw_channel = cw_processes[s].channel[1];

    } else {
        cw_processes[s].channel[0] = -1;
        cw_processes[s].channel[1] = -1;
    }

    cw_process_slot = s;

    pid = fork();

    switch (pid) {

    case -1:
        cw_log_fatal(cycle->log, "fork() failed while spawning \"%s\"", name);
        cw_close_channel(cw_processes[s].channel, cycle->log);
        return CW_INVALID_PID;

    case 0:
        cw_pid = cw_getpid();
        proc(cycle, data);
        break;

    default:
        break;
    }

    cw_log_notice(cycle->log, "start %s %d", name, (pid_t)pid);

    cw_processes[s].pid = pid;
    cw_processes[s].exited = 0;

    if (respawn >= 0) {
        return pid;
    }

    cw_processes[s].proc = proc;
    cw_processes[s].data = data;
    cw_processes[s].name = name;
    cw_processes[s].exiting = 0;

    switch (respawn) {

    case CW_PROCESS_NORESPAWN:
        cw_processes[s].respawn = 0;
        cw_processes[s].just_spawn = 0;
        cw_processes[s].detached = 0;
        break;

    case CW_PROCESS_JUST_SPAWN:
        cw_processes[s].respawn = 0;
        cw_processes[s].just_spawn = 1;
        cw_processes[s].detached = 0;
        break;

    case CW_PROCESS_RESPAWN:
        cw_processes[s].respawn = 1;
        cw_processes[s].just_spawn = 0;
        cw_processes[s].detached = 0;
        break;

    case CW_PROCESS_JUST_RESPAWN:
        cw_processes[s].respawn = 1;
        cw_processes[s].just_spawn = 1;
        cw_processes[s].detached = 0;
        break;

    case CW_PROCESS_DETACHED:
        cw_processes[s].respawn = 0;
        cw_processes[s].just_spawn = 0;
        cw_processes[s].detached = 1;
        break;
    }

    if (s == cw_last_process) {
        cw_last_process++;
    }

    return pid;
}