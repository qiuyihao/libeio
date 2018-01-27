/**
 * compile with tcmalloc, command by follows:
 *  definition: #define USE_TCMALLOC
 *  makefile: -L -ltcmalloc 
 */

#ifndef __EIO_BUFFER_POOL_H_INCLUDED__
#define __EIO_BUFFER_POOL_H_INCLUDED__

#include "eio.h"
#include "eio_ptrlist.h"
#include "eio_debug.h"


/* Explicitly override malloc/free etc when using tcmalloc. */
#if defined(USE_TCMALLOC)
  #include <gperftools/tcmalloc.h>
  #define eio_malloc(size) tc_malloc(size)
  #define eio_calloc(count,size) tc_calloc(count,size)
  #define eio_realloc(ptr,size) tc_realloc(ptr,size)
  #define eio_free(ptr) tc_free(ptr)
#elif defined(USE_JEMALLOC)
  #define eio_malloc(size) je_malloc(size)
  #define eio_calloc(count,size) je_calloc(count,size)
  #define eio_realloc(ptr,size) je_realloc(ptr,size)
  #define eio_free(ptr) je_free(ptr)
#else
  #define eio_malloc(size) ::malloc(size)
  #define eio_calloc(count,size) ::calloc(count,size)
  #define eio_realloc(ptr,size) ::realloc(ptr,size)
  #define eio_free(ptr) ::free(ptr)
#endif


/* EIO binary buffer object, it only contains buffer and pointer to begin and end position,
   set both of pointer to the memory address of buffer when reset to use. */
class Tcmalloc_Buffer
{
public:
    Tcmalloc_Buffer();
    virtual ~Tcmalloc_Buffer();

    /// malloc & free buffer section.
    int malloc(int buflen);
    int free();

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
    inline int get_remain_buffer_len(){ return (__buflimit - __end); }

public:
    char* __begin;
    char* __end;

private:
    char* __buf;
    char* __buflimit;
};


/* Pool of Buffer by list. */
class Tcmalloc_Buffer_Pool
{
public:
    Tcmalloc_Buffer_Pool();
    virtual ~Tcmalloc_Buffer_Pool();

    Tcmalloc_Buffer* allocate(int buflen);
    void release(Tcmalloc_Buffer* buffer);

private:
    EIO_PtrList<Tcmalloc_Buffer*> __buflist;
};

#endif
