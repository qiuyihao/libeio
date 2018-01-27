#include "eio_string.h"
#include <iconv.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char* str_replace(char* src, char oldChar, char newChar)
{
    char* head = src;

    while(*src != '\0') {
        if(*src==oldChar) *src = newChar;
        src++;
    }

    return head;
}


int strsplit_num(char* str, const char* del)
{
    char *first = NULL;
    char *second = NULL;
    int num = 0;

    first = strstr(str, del);
    while(first != NULL)
    {
        second = first+1;
        num ++;
        first = strstr(second, del);
    }
    return num;
}


int strlen_utf8(char* s)
{
    int i = 0, j = 0;
    while(s[i]) {
        if((s[i] & 0xc0) != 0x80) j++;
        i ++;
    }
    return j;
}


int hextoi(char* hex)
{
    int i;
    sscanf(hex, "%x", &i);
    return i;
}

