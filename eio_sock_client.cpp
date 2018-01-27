#include "eio_sock_client.h"


EIO_Sock_Client::EIO_Sock_Client(EIO_Events_Listener* evt_listener, EIO_Buffer_Pool* buf_pool, int readbuf_bytes)
    : __evtlistener(evt_listener),
      __bufpool(buf_pool),
      __readbuf(readbuf_bytes)
{
}


EIO_Sock_Client::~EIO_Sock_Client()
{
}


int EIO_Sock_Client::init(int fd)
{
    this->__fd = fd;
    this->__keepalive = 0;
    this->read_buffer_reset();
    return 1;
}


int EIO_Sock_Client::uninit()
{
    this->read_buffer_reset();
    this->write_queue_reset();
    this->__fd = 0;
    this->__keepalive = 0;
    return 1;
}


void EIO_Sock_Client::read_buffer_reset()
{
    this->__readbuf.reset_pointer();
}


void EIO_Sock_Client::write_queue_reset()
{
    EIO_Buffer* buffer;

    for(; ;)
    {
        if(!__wqueue.Front(buffer))
            break;

        __wqueue.Pop();
        __bufpool->release(buffer);
    }
}


int EIO_Sock_Client::read()
{
    // read stream from socket.
    int readlen = libsocket_nonblocking_read(__fd, __readbuf.get_buffer_end(), __readbuf.get_buffer_remain_len()-4); // -4 to small than remain buffer.

    // less than zero, it means the connection just has some error, maybe shutdown, etc.
    // if it be zero, kernel error is detected, but connection still valid.
    if(readlen < 0)
    {
        return -1;
    }
    else if(readlen == 0)
    {
        this->epoll_ctl_modify();
        return 1;
    }
    else
    {
        __readbuf.step_end_position(readlen);

        // decode buffer with Http-Protocol method.
        if(this->read_complete(&__readbuf) < 0) {
            return -1;
        }

        return 1;
    }
}


int EIO_Sock_Client::write()
{
    EIO_Buffer* buffer = NULL;
    int buflen = 0;
    int writelen = 0;

    for(; ;)
    {
        if(!__wqueue.Front(buffer)) {
            goto WRITE_OK;
        }

        buflen = buffer->get_buffer_len();
        writelen = libsocket_nonblocking_write(__fd, buffer->get_buffer_begin(), buflen);

        // less than zero, it means the connection just has some error, maybe shutdown, etc.
        // if it be zero, kernel error is detected, but connection still valid.
        if(writelen < 0)
        {
            return -1;
        }
        else if(writelen == 0)
        {
            goto WRITE_OK;
        }
        else
        {
            if(writelen < buflen)
            {
                buffer->step_begin_position(writelen);
                goto WRITE_OK;
            }
            else {
                __wqueue.Pop();
                __bufpool->release(buffer);
            }
        }
    }

WRITE_OK:
    /// rebind epoll event to socket fd.
    this->epoll_ctl_modify();
    return 1;
}


int EIO_Sock_Client::write(char* buf, int buflen)
{
    EIO_Buffer* buffer;
    int writelen;
    int memcpylen;

    /// push buffer to client write queue if send no complete.
    if(__wqueue.Size() == 0)
    {
        writelen = libsocket_nonblocking_write(__fd, buf, buflen);

        if(writelen == buflen) {
            this->epoll_ctl_modify();
            return 1;
        }
        else if(writelen < 0) {
            return -1;
        }
    }

    buf += writelen;
    buflen -= writelen;

    /// push unwrite buffer to client write queue.
    while(buflen > 0)
    {
        buffer = __bufpool->allocate();
        if(buffer == NULL) {
            this->epoll_ctl_modify();
            return 1;
        }

        memcpylen = MIN(buffer->get_buffer_remain_len()-4, buflen);
        memcpy(buffer->get_buffer(), buf, memcpylen);
        buffer->set_begin_position(0);
        buffer->set_end_position(memcpylen);
        __wqueue.Push(buffer);

        buf += memcpylen;
        buflen -= memcpylen;

        //memcpy(buffer->GetBuffer(), buf+writelen, buflen-writelen);
        //buffer->__begin = buffer->GetBuffer();
        //buffer->__end = buffer->GetBuffer() + (buflen-writelen);
        //__wqueue.Push(buffer);
    }

WRITE_OK:
    this->epoll_ctl_modify();
    return 1;
}


/* Epoll event attach and modify is depended with epoll_ctl, you could waiting for
   socket io event by kernel level, and kernel will notify to caller. */
int EIO_Sock_Client::epoll_ctl_attach()
{
    __evtlistener->epoll_ctl_attach(__fd, __fd, EPOLLIN|EPOLLET);
    return 1;
}

int EIO_Sock_Client::epoll_ctl_modify()
{
    uint32 events = (__wqueue.Size()==0) ? (EPOLLIN|EPOLLET) : (EPOLLIN|EPOLLOUT|EPOLLET);
    __evtlistener->epoll_ctl_modify(__fd, __fd, events);
    return 1;
}


int EIO_Sock_Client::epoll_ctl_delete()
{
    return 1;
}


int EIO_Sock_Client::read_complete(EIO_Buffer* buffer)
{
    return 0;
}
