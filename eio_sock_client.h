#ifndef __EIO_SOCK_CLIENT_H_INCLUDED__
#define __EIO_SOCK_CLIENT_H_INCLUDED__

#include "eio.h"
#include "eio_ptrlist.h"
#include "eio_buffer_pool.h"
#include "eio_events_listener.h"


class EIO_Sock_Client
{
public:
    EIO_Sock_Client(EIO_Events_Listener* evt_listener, EIO_Buffer_Pool* buf_pool, int readbuf_bytes);
    virtual ~EIO_Sock_Client();

    /// initialize and uninitialize client object.
    int init(int fd);
    int uninit();

    /// read & write from remote socket.
    int read();
    int write();
    int write(char* buf, int buflen);

protected:
    /// epoll event binding operations.
    int epoll_ctl_attach();
    int epoll_ctl_modify();
    int epoll_ctl_delete();

    void write_queue_reset();
    void read_buffer_reset();

    virtual int read_complete(EIO_Buffer* buffer);

public:
    EIO_Events_Listener* evtlistener_get(){ return __evtlistener; }
    void keepalive_set(int keepalive){ __keepalive = keepalive; }
    int keepalive_get(){ return __keepalive; }

protected:
    EIO_Events_Listener* __evtlistener;
    EIO_Buffer_Pool* __bufpool;

    int __fd;
    int __keepalive;

    EIO_Lock_Mutex __lock;
    EIO_Buffer __readbuf;
    EIO_PtrList<EIO_Buffer*> __wqueue;
};

#endif
