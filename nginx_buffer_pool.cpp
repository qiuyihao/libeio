#include "nginx_buffer_pool.h"
#include <stdio.h>

Nginx_Buffer_Pool::Nginx_Buffer_Pool()
{
}


Nginx_Buffer_Pool::~Nginx_Buffer_Pool()
{
}

static void *palloc_block(nginx_pool_t *pool, size_t size);


nginx_pool_t* Nginx_Buffer_Pool::create_pool(size_t size)
{
    nginx_pool_t* p;
    p = (nginx_pool_t*)nginx_memalign(POOL_ALIGNMENT, size);

    if(p == NULL) {
        return p;
    }

    p->d.last = (u_char *) p + sizeof(nginx_pool_t);
    p->d.end = (u_char *)p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = p->d.end - p->d.last;
    p->max = size;

    p->current = p;
    __pool = p;
    return p;
}


void* Nginx_Buffer_Pool::palloc(size_t size)
{
    void *m;
    nginx_pool_t *pool = __pool;
    nginx_pool_t *p = pool->current;

    if(size >= p->max) {
        return NULL;
    }

    do
    {
        if(p->d.end - p->d.last > size)
        {
            m = p->d.last;
            p->d.last += size;
            return m;
        }
        p = p->d.next;

    } while(p);

    return palloc_block(pool, size);
}


static void* palloc_block(nginx_pool_t *pool, size_t size)
{
    void *m;
    nginx_pool_t *new_pool, *p, *current;
    size_t pool_size;

    pool_size = pool->d.end - (u_char *)pool;
    new_pool = (nginx_pool_t*)nginx_memalign(POOL_ALIGNMENT, pool_size);

    if(new_pool == NULL) {
        return NULL;
    }

    // init new_pool
    new_pool->d.last = (u_char *)new_pool + sizeof(nginx_pool_t);
    new_pool->d.end = (u_char *)new_pool + pool_size;
    new_pool->d.next = NULL;
    new_pool->d.failed = 0;
    new_pool->max = new_pool->d.end - new_pool->d.last;

    // 因为原来的pool_t不能分配才到这个函数，所以从current开始的pool_t块都failed加1
    // 当pool->current指向的块的failed超过4时，current指向下一个块
    // 其实前面的块失败次数永远  >= 后面的块，所以当一个块failed < 4时，最后的块也不会改变current了
    current = pool->current;
    for(p = current; p->d.next; p = p->d.next)
    {
        if(p->d.failed++ > 4)
        {
            current = p->d.next;
        }
    }

    // 无论如何，new_pool都是在单链表的尾部，上面的p沿current开始直到尾部
    p->d.next = new_pool;

    // 如果当前的current指向了原本链表最后一块的d.next，那么
    pool->current = current ? current : new_pool;

    m = new_pool->d.last;
    new_pool->d.last += size;
    return m;
}


void Nginx_Buffer_Pool::get_pool_status()
{
    int n = 0;
    nginx_pool_t *p = __pool;

    printf("**********************************************************************\n");
    for(; p; p = p->d.next, n++)
    {
        printf("pool:%d  ", n);
        printf("max:%d  left:%d\n", p->max, p->d.end - p->d.last);
    }
    printf("**********************************************************************\n");
}


int Nginx_Buffer_Pool::pool_reset()
{
    nginx_pool_t *pool = __pool;
    nginx_pool_t *pp = pool;
    pool->current = pool;

    for(; pp ; pp = pp->d.next)
    {
        pp->d.last = (u_char*)pp + sizeof(nginx_pool_t);
        pp->d.failed = 0;
    }

    return 0;
}


int Nginx_Buffer_Pool::pool_destory()
{
    nginx_pool_t *pp, *p;
    pp = __pool;
    p = pp->d.next;

    for(; pp ; pp = p, p = pp->d.next)
    {
        pp->d.next = NULL;
        free(pp);

        if(p == NULL) {
            break;
        }
    }

    return 0;
}


void* nginx_memalign(size_t alignment, size_t size)
{
    void *p;
    int err;
    err = posix_memalign(&p, alignment, size);
    return p;
}
