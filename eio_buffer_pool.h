#ifndef __EIO_BUFFER_POOL_H_INCLUDED__
#define __EIO_BUFFER_POOL_H_INCLUDED__

#include "eio.h"
#include "eio_ptrlist.h"


/* EIO binary buffer object, it only contains buffer and pointer to begin and end position,
   set both of pointer to the memory address of buffer when reset to use. */
class EIO_Buffer
{
public:
    EIO_Buffer(int buflen);
    virtual ~EIO_Buffer();

    void reset_pointer();
    void zero_buffer();

    inline void set_begin_position(int pos){ __begin = __buf + pos; }
    inline void set_end_position(int pos){ __end = __buf + pos; }
    inline void step_begin_position(int step){ __begin += step; }
    inline void step_end_position(int step){ __end += step; }
    inline char* get_buffer_begin(){ return __begin; }
    inline char* get_buffer_end(){ return __end; }
    inline char* get_buffer(){ return __buf; }
    inline int get_buffer_len(){ return (__end - __begin); }
    inline int get_buffer_remain_len(){ return (__buflimit - __end); }

private:
    char* __begin;
    char* __end;
    char* __buf;
    char* __buflimit;
};


/* Pool of Buffer by list. */
class EIO_Buffer_Pool
{
public:
    EIO_Buffer_Pool(int buflen);
    virtual ~EIO_Buffer_Pool();

    EIO_Buffer* allocate();
    void release(EIO_Buffer* buffer);

private:
    int __buflen;
    EIO_PtrList<EIO_Buffer*> __buflist;
};

#endif
