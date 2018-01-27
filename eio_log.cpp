#include "eio_log.h"
#include "eio_util.h"

#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>


static void init_file_path(eio_log_t* log);


void eio_log_debug(eio_log_t* log, int level, char* file, char* function, int line, const char *fmt, ...)
{
    int i;
    va_list args;
    char printbuf[512+4];

    va_start(args, fmt);
    i = vsnprintf(printbuf, 512, fmt, args);
    printbuf[i] = 0;
    va_end(args);

    switch(level)
    {
    case EIO_LOG_NOTICE:
        printf("[%s]%s(%d): %s\n", file, function, line, printbuf);
        break;
    default:
        eio_log_stderr(log, file, function, line, printbuf);
        break;
    }
}


void eio_log_stderr(eio_log_t* log, char* file, char* function, int line, char* text)
{
    if(!log->dbg_term && !log->dbg_file)
        return;

    /// print to screen.
    if(log->dbg_term)
    {
        printf("[%s]%s(%d): %s\n", file, function, line, text);
    }

    /// write into file.
    if(log->dbg_file)
    {
        if(log->fp_log == NULL)
            return;
        time_t t = time(NULL);
        struct tm* tm1 = localtime(&t);
        if(!tm1) return;
        if(tm1->tm_mday != log->log_day){
            init_file_path(log);
        }

        char tmp[16];
        strftime(tmp, 15, "%X", tm1);
        fprintf(log->fp_log, "%s [%s]%s(%d): %s\n", tmp, file, function, line, text);
        fflush(log->fp_log);
    }
}


static char* hex_str(unsigned char *buf, int len, char* outstr )
{
    const char *set = "0123456789abcdef";
    char *tmp;
    unsigned char *end;
    if (len > 1024)
    len = 1024;
    end = buf + len;
    tmp = &outstr[0];
	while (buf < end)
	{
        *tmp++ = set[ (*buf) >> 4 ];
        *tmp++ = set[ (*buf) & 0xF ];
        *tmp++ = ' ';
        buf ++;
    }
    *tmp = '\0';
    return outstr;
}


void hex_dump(eio_log_t* log, unsigned char * buf, int len )
{
    char str[KB(4)];
    if(log->dbg_term) {
        puts(hex_str(buf, len, str));
    }
    if(log->dbg_file) {
        fputs(hex_str(buf, len, str), log->fp_log);
        fprintf(log->fp_log, "\n");
        fflush(log->fp_log);
    }
    //fprintf(stderr, hex_str(buf, len));
}


void eio_log_term_on(eio_log_t* log)
{
    log->dbg_term = 1;
}


void eio_log_term_off(eio_log_t* log)
{
    log->dbg_term = 0;
}


void init_file_path(eio_log_t* log)
{
    char tmp[64];
    time_t t = time(NULL);
    struct tm* tm1 = localtime(&t);
    if(!tm1) {
        perror("log.c init_file_path: ERROR GETTING SYSTEM TIME.");
    }
    log->log_day = tm1->tm_mday;
    strftime(tmp, 64, "/%Y-%m-%d.txt", tm1);
    if(access(log->dir, 0) != 0) {
        mkdir_recursive(log->dir);
    }
    strcpy(log->filename, log->dir);
    strcat(log->filename, tmp);
    if(log->fp_log)
        fclose(log->fp_log);
    log->fp_log = fopen(log->filename, "aw");
    //fp_log = fopen( filename, "w" );
}


void eio_log_file_on(eio_log_t* log)
{
    if(log->dbg_file)
        return;
    init_file_path(log);
    log->dbg_file = 1;
}


void eio_log_file_off(eio_log_t* log)
{
    if(log->dbg_file)
    {
        log->dbg_file = 0;
        if(log->fp_log) {
            fclose(log->fp_log);
        }
    }
}


void eio_log_set_dir(eio_log_t* log, char* str)
{
    strcpy(log->dir, str);
}

