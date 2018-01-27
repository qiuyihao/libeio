#ifndef __NGINX_BUFFER_POOL_H_INCLUDED__
#define __NGINX_BUFFER_POOL_H_INCLUDED__

#include <stdlib.h>

#define POOL_ALIGNMENT 16

typedef unsigned char u_char;
typedef struct nginx_pool_data_s nginx_pool_data_t;
typedef struct nginx_pool_s nginx_pool_t;

struct nginx_pool_data_s
{
    u_char *last;
    u_char *end;
    nginx_pool_t *next;
    int failed;
};

struct nginx_pool_s
{
    nginx_pool_data_t d;
    size_t max;
    nginx_pool_t *current;
};


void *nginx_memalign(size_t alignment, size_t size);


class Nginx_Buffer_Pool
{
public:
    Nginx_Buffer_Pool();
    virtual ~Nginx_Buffer_Pool();

    nginx_pool_t *create_pool(size_t size);

    void* pool_alloc(size_t size);
    void* pool_calloc(size_t size);

    void* palloc(size_t size);
    void* pnalloc(size_t size);

    int pool_destory();
    int pool_reset();

    /// nginx pool status.
    void get_pool_status();

private:
    nginx_pool_t* __pool;
};

#endif
