#ifndef __EIO_LOG_H_INCLUDED__
#define __EIO_LOG_H_INCLUDED__

#include "eio.h"
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define EIO_LOG_STDERR            0
#define EIO_LOG_EMERG             1
#define EIO_LOG_ALERT             2
#define EIO_LOG_CRIT              3
#define EIO_LOG_ERR               4
#define EIO_LOG_WARN              5
#define EIO_LOG_NOTICE            6
#define EIO_LOG_INFO              7
#define EIO_LOG_DEBUG             8

typedef struct eio_log_s eio_log_t;

struct eio_log_s {
    int dbg_term;
    int dbg_file;
    int log_day;
    char dir[128];
    char filename[160];
    FILE* fp_log;
};

void eio_log_debug(eio_log_t* log, int level, char* file, char* function, int line, const char *fmt, ...);
void eio_log_stderr(eio_log_t* log, char* file, char* function, int line, char* text);
void eio_log_hex_dump(eio_log_t* log, unsigned char * buf, int len);
void eio_log_term_on(eio_log_t* log);
void eio_log_term_off(eio_log_t* log);
void eio_log_file_on(eio_log_t* log);
void eio_log_file_off(eio_log_t* log);
void eio_log_set_dir(eio_log_t* log, char* str);

#endif /* __EIO_LOG_H_INCLUDED__ */

