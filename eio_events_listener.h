#ifndef __EIO_EVENTS_LISTENER_H_INCLUDED__
#define __EIO_EVENTS_LISTENER_H_INCLUDED__

#include "eio.h"
#include "libepoll.h"


/* High-Performance Server architecture.
 */
class EIO_Events_Listener
{
public:
    EIO_Events_Listener();
    virtual ~EIO_Events_Listener();

    int create(int epsize);
    int run();
    void shutdown();

    void epoll_ctl_attach(int fd, int evfd, uint32 events);
    void epoll_ctl_modify(int fd, int evfd, uint32 events);

protected:
    static int epoll_event_dispatch(struct epoll_event* ev, void* args);
    virtual int epoll_event_handler(uint32 evts, int evfd) = 0;

private:
    libepoll epoll;
};

#endif
