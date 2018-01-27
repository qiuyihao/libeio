#ifndef __LIBSTRING_H_INCLUDED__
#define __LIBSTRING_H_INCLUDED__

#include <stdint.h>

int strlen_utf8(char* s);
char* str_replace(char* src, char oldChar, char newChar);
int strsplit_num(char* str, const char* del);
int hextoi(char* hex);

#endif

