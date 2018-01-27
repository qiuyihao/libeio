#include "eio_thread.h"


int EIO_Thread::run()
{
    if(::pthread_create(&pthread, NULL, threadwrapper, this) != 0)
        return -1;

    return 0;
}


void* EIO_Thread::threadwrapper(void* args)
{
    EIO_Thread* that = (EIO_Thread*)args;
    that->thread();
    return NULL;
}



/**
 * thread pool.
 */
EIO_Thread_Pool::EIO_Thread_Pool()
    : poolsize(0),
      pthreads(NULL)
{
}


EIO_Thread_Pool::~EIO_Thread_Pool()
{
}


int EIO_Thread_Pool::run(int num)
{
    if(poolsize != 0)
        return -1;

    poolsize = num;
    pthreads = new pthread_t[poolsize];

    /* create num thread. */
    for(int index=0; index<poolsize; index++)
    {
        if(::pthread_create(&pthreads[index], NULL, threadwrapper, this) != 0) {
        }
    }

    return 1;
}


int EIO_Thread_Pool::shutdown()
{
    /* cancel thread of pool. */
    for(int index=0; index<poolsize; index++) {
        ::pthread_cancel(pthreads[index]);
    }

    delete pthreads;
    pthreads = NULL;
    poolsize = 0;

    return 1;
}


void* EIO_Thread_Pool::threadwrapper(void* args)
{
    EIO_Thread_Pool* that = (EIO_Thread_Pool*)args;
    that->thread();
    return NULL;
}

