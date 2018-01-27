#ifndef __EIO_PTRLIST_H__
#define __EIO_PTRLIST_H__

#include <queue>
using namespace std;

#include "eio_lock.h"


/*
 * pointer list with STL, written by eyhel
 */
template <typename T>
class EIO_PtrList
{
public:
    EIO_PtrList()
    {
    }

    virtual ~EIO_PtrList()
    {
    }

    bool Front(T& v)
    {
        _lock.lock();
        if(_queue.size() == 0) {
            _lock.unlock();
            return false;
        }

        v = _queue.front();
        _lock.unlock();
        return true;
    }

    void Push(T& v)
    {
        _lock.lock();
        _queue.push(v);
        _lock.unlock();
    }

    void Pop()
    {
        _lock.lock();
        _queue.pop();
        _lock.unlock();
    }

    int Size()
    {
        int size = 0;
        _lock.lock();
        size = _queue.size();
        _lock.unlock();
        return size;
    }

private:
    queue<T> _queue;
    EIO_Lock_Mutex _lock;
};

#endif
