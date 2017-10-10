#include <cw_config.h>
#include <cw_core.h>

int
cw_nonblocking(cw_socket_t s)
{
    int  nb;

    nb = 1;

    return ioctl(s, FIONBIO, &nb);
}


int
cw_blocking(cw_socket_t s)
{
    int  nb;

    nb = 0;

    return ioctl(s, FIONBIO, &nb);
}