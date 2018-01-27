#include "eio_events_listener.h"


EIO_Events_Listener::EIO_Events_Listener()
{
}


EIO_Events_Listener::~EIO_Events_Listener()
{
    libepoll_close(&epoll);
}


int EIO_Events_Listener::create(int epsize)
{
    /* create epoll FD. file descriptor with custom epoll size,
       bind listen event and waiting to dispatch. */
    if(libepoll_create(&epoll, epsize) < 0) {
        return -1;
    }

    return 0;
}


int EIO_Events_Listener::run()
{
    libepoll_dispatch(&epoll, epoll_event_dispatch, this);
    return 0;
}


void EIO_Events_Listener::shutdown()
{
    libepoll_close(&epoll);
}


/** It detected socket IO event and callback this Handle with actions
    Accept/Read/Write, the events include EPOLLIN/EPOLLOUT. */
int EIO_Events_Listener::epoll_event_dispatch(struct epoll_event* ev, void* args)
{
    EIO_Events_Listener* that = (EIO_Events_Listener*)args;
    int16 evts = ev->events;
    int evfd = ev->data.fd;
    return (that->epoll_event_handler(evts, evfd));
}


/** attach new event to epoll listen center. */
void EIO_Events_Listener::epoll_ctl_attach(int fd, int evfd, uint32 events)
{
    libepoll_event_attach(&epoll, EPOLL_CTL_ADD, fd, evfd, events);
}

/** modify a exist event to epoll lister center. */
void EIO_Events_Listener::epoll_ctl_modify(int fd, int evfd, uint32 events)
{
    libepoll_event_attach(&epoll, EPOLL_CTL_MOD, fd, evfd, events);
}
