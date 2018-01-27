#ifndef __EIO_SEMLIST_H_INCLUDED__
#define __EIO_SEMLIST_H_INCLUDED__

#include <semaphore.h>
#include <list>

#include "eio_lock.h"


template<typename Type>
class EIO_Sem_List : public list<Type>, public EIO_Lock_Mutex
{
public:
    EIO_Sem_List()
    {
        ::sem_init(&_sem, 0, 0);
    }

    ~EIO_Sem_List()
    {
    }

    void push_back2(Type& item)
    {
        push_back(item);
        ::sem_post(&_sem);
    }

    void waitsem()
    {
        ::sem_wait(&_sem);
    }

private:
    sem_t _sem;
};

#endif /* __EIO_SEMLIST_H_INCLUDED__ */

