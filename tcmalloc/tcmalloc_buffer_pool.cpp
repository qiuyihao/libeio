#include "tcmalloc_buffer_pool.h"


Tcmalloc_Buffer::Tcmalloc_Buffer()
{
}

Tcmalloc_Buffer::~Tcmalloc_Buffer()
{
}


int Tcmalloc_Buffer::malloc(int buflen)
{
    char* buf = (char*)eio_calloc(1, buflen);
    //char* buf = (char*)eio_malloc(buflen);
    //buf->zero_buffer();

    if(buf == NULL)
        return -1;

    this->__buf = buf;
    this->__begin = this->__end = this->__buf;
    this->__buflimit = this->__buf + buflen;
    return 1;
}


int Tcmalloc_Buffer::free()
{
    eio_free(this->__buf);
    this->__begin = this->__end = NULL;
    this->__buf = this->__buflimit = NULL;
    return 1;
}


void Tcmalloc_Buffer::reset_pointer()
{
    __begin = __end = __buf;
}


void Tcmalloc_Buffer::zero_buffer()
{
    memset(__buf, 0, (__buflimit - __buf));
}


////////////////////////////////////////////////////////////////////////////////////


Tcmalloc_Buffer_Pool::Tcmalloc_Buffer_Pool()
{
}


Tcmalloc_Buffer_Pool::~Tcmalloc_Buffer_Pool()
{
}


/* Allocate and Release buffer memory object, and it is
   support virtual buffer linklist. */
Tcmalloc_Buffer* Tcmalloc_Buffer_Pool::allocate(int buflen)
{
    Tcmalloc_Buffer* buffer = NULL;

    /// if list is empty, allocate real memory for new buffer.
    if(__buflist.Front(buffer)) {
        __buflist.Pop();
    } else {
        buffer = new Tcmalloc_Buffer();
    }

    /// if buffer is null, log information like about out of memory.
    if(buffer == NULL) {
        EIO_Log("### server doesn't work because of out of memory storage");
        return NULL;
    }

    if(buffer->malloc(buflen) < 0) {
        __buflist.Push(buffer);
        return NULL;
    }

    return buffer;
}


void Tcmalloc_Buffer_Pool::release(Tcmalloc_Buffer *buffer)
{
    buffer->free();
    __buflist.Push(buffer);
}
