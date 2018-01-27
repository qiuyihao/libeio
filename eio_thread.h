#ifndef __EIO_THREAD_H_INCLUDED__
#define __EIO_THREAD_H_INCLUDED__

#include <pthread.h>


/* eio thread. */
class EIO_Thread
{
public:
    EIO_Thread(){}
    virtual ~EIO_Thread(){}

    int run();

protected:
    static void* threadwrapper(void* args);
    virtual int thread() = 0;

private:
    pthread_t pthread;
};


/* eio thread pool. */
class EIO_Thread_Pool
{
public:
    EIO_Thread_Pool();
    virtual ~EIO_Thread_Pool();

    int run(int num);
    int shutdown();

protected:
    static void* threadwrapper(void* args);
    virtual int thread() = 0;

private:
    int poolsize;
    pthread_t* pthreads;
};

#endif
