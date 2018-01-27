#include "eio_buffer_pool.h"


EIO_Buffer::EIO_Buffer(int buflen)
{
    this->__buf = new char[buflen];
    this->__begin = this->__end = this->__buf;
    this->__buflimit = this->__buf + buflen;
}

EIO_Buffer::~EIO_Buffer()
{
}


void EIO_Buffer::reset_pointer()
{
    this->__begin = this->__end = this->__buf;
}


void EIO_Buffer::zero_buffer()
{
    memset(__buf, 0, (__buflimit - __buf));
}


//////////////////////////////////////////////////////////////////////////


EIO_Buffer_Pool::EIO_Buffer_Pool(int buflen)
{
    __buflen = buflen;
}


EIO_Buffer_Pool::~EIO_Buffer_Pool()
{
}


/* Allocate and Release buffer memory object, and it is
   support virtual buffer linklist. */
EIO_Buffer* EIO_Buffer_Pool::allocate()
{
    EIO_Buffer* buffer = NULL;

    /* if list is empty, allocate real memory for new buffer. */
    if(__buflist.Front(buffer))
        __buflist.Pop();
    else
        buffer = new EIO_Buffer(__buflen);

    if(buffer == NULL) {
        cout<<"### server doesn't work because of out of memory storage"<<endl;
    }

    buffer->reset_pointer();
    return buffer;
}


void EIO_Buffer_Pool::release(EIO_Buffer* buffer)
{
    __buflist.Push(buffer);
}
