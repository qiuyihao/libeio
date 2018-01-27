#include "libepoll.h"
#include <errno.h>


int libsocket_listen(short port, int backlog)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
        return -1;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(fd, (const sockaddr*)&addr, sizeof(addr)) < 0)
        return -1;

    if(listen(fd, backlog) < 0)
        return -1;

    return fd;
}


int libsocket_nonblocking_read(int fd, char* buf, int len)
{
    int readlen = read(fd, buf, len);

    //* if read length is less than zero, then error number isn't EAGAIN,
    //* it means the connections failed, the same to zero result.
    if(readlen == 0) {
        return -1;
    }
    else if(readlen < 0)
    {
        if(errno != EAGAIN)
            return -1;
        else
            return 0;
    }

    return readlen;
}


int libsocket_nonblocking_write(int fd, char* buf, int len)
{
    if(buf==NULL || len<=0)
        return 0;

    int writelen = write(fd, buf, len);

    //* if read length is less than zero, then error number isn't EAGAIN,
    //* it means the connections failed, the same to zero result.
    if(writelen == 0) {
        return -1;
    }
    else if(writelen < 0)
    {
        if(errno != EAGAIN)
            return -1;
        else
            return 0;
    }

    return writelen;
}


int libsocket_setnonblocking(int fd)
{
    int flags;
    flags = fcntl(fd, F_GETFL);
    if(flags < 0)
        return flags;

    flags |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flags) < 0)
        return -1;

    return 0;
}


int libepoll_create(struct libepoll* __libepoll, int __epsize)
{
    int epfd;

    // create epoll file descriptor FD
    epfd = epoll_create(__epsize);
    if(epfd < 0)
        return -1;

    // copy interface data to libepoll structure
    __libepoll->epsize = __epsize;
    __libepoll->epfd = epfd;

    return 0;
}


int libepoll_event_attach(struct libepoll* __libepoll, int __op, int __fd, int evfd, uint32 events)
{
    struct epoll_event epevent;
    epevent.events = events;
    epevent.data.fd = evfd;
    epoll_ctl(__libepoll->epfd, __op, __fd, &epevent);
}


int libepoll_dispatch(struct libepoll* __libepoll, int (*libepoll_dispatch_callback)(struct epoll_event*, void*), void* args)
{
    int epfd = __libepoll->epfd;
    int epsize = __libepoll->epsize;
    struct epoll_event epevents[epsize];
    int nfds;

    for(; ;)
    {
        nfds = epoll_wait(epfd, epevents, epsize, 500);

        for(int index = 0; index < nfds; index ++)
        {
            libepoll_dispatch_callback(&epevents[index], args);
        }
    }

    return 0;
}


void libepoll_close(struct libepoll* __libepoll)
{
    close(__libepoll->epfd);
}
