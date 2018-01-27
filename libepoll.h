#ifndef __LIBEPOLL_H__
#define __LIBEPOLL_H__

#include <unistd.h>
#include <stdint.h>

#include <iostream>
using namespace std;

#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

typedef  unsigned int  uint32;


/* libepoll configure and event */
struct libepoll
{
    int epsize;
    int epfd;
};


/*
 */
int libsocket_listen(short port, int backlog);

/*
 */
int libsocket_nonblocking_read(int fd, char* buf, int len);
int libsocket_nonblocking_write(int fd, char* buf, int len);


/* Set socket file descriptor FD as non-blocking model,
   Return 0 in case of success.  */
int libsocket_setnonblocking(int fd);


/* Create a new epoll file descriptor FD, fill libepoll configure
   structure, Returns 0 in case of success, otherwise -1.  */
int libepoll_create(struct libepoll* __libepoll, int __epsize);


/* Epoll event binding.  */
int libepoll_event_attach(struct libepoll* __libepoll, int __op, int __fd, int evfd, uint32 events);


/* Dispatch event for Epoll listen. */
int libepoll_dispatch(struct libepoll* __libepoll, int (*libepoll_dispatch_callback)(struct epoll_event*, void*), void*);

/* Close an exist libepoll file descriptor FD. */
void libepoll_close(struct libepoll* __libepoll);

#endif
