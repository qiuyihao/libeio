#ifndef __EIO_LOCK_H_INCLUDED__
#define __EIO_LOCK_H_INCLUDED__

#include <pthread.h>


/* linux protect lock of mutex. */
class EIO_Lock_Mutex
{
public:
    EIO_Lock_Mutex()
    {
        pthread_mutex_init(&_mutex, NULL);
    }

    virtual ~EIO_Lock_Mutex()
    {
        pthread_mutex_destroy(&_mutex);
    }

    inline void lock()
    {
        pthread_mutex_lock(&_mutex);
    }

    inline void unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }

private:
    pthread_mutex_t _mutex;
};

#endif

