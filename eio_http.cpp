#include "eio_http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Get next token from string *stringp, where tokens are possibly-empty
 * strings separated by characters from delim.
 *
 * Writes NULs into the string at *stringp to end tokens.
 * delim need not remain constant from call to call.
 * On return, *stringp points past the last NUL written (if there might
 * be further tokens), or is NULL (if there are definitely no more tokens).
 *
 * If *stringp is NULL, strsep returns NULL.
 */
char* strsep(char **stringp, const char *delim)
{
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;

    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}


int EIO_HttpParser::head_len(char* http_data)
{
    char* line2_pos = NULL;
    int line2_len = strlen("\r\n\r\n");
    int head_len = 0;

    if((line2_pos = strstr(http_data, "\r\n\r\n")) == NULL)
        return 0;

    head_len = (line2_pos - http_data + line2_len);
    return (head_len);
}


void EIO_HttpParser::uri_get(const char* head_data, char* method, char* uri, char* version)
{
    char temp[256];
    char* p = strstr(head_data, "\r\n");

    if(p == NULL) {
        strcpy(method, "");
        strcpy(uri, "");
        strcpy(version, "");
        return;
    }

    strncpy(temp, head_data, p-head_data);
    sscanf(temp, "%s %s %s", method, uri, version);
    return;
}


void EIO_HttpParser::head_get(char* head_data, char* name, char* value, int len)
{
    char *key_pos, *value_pos;
    int key_len=strlen(name), value_len, line2_len=strlen("\r\n\r\n");

    /* find http-protocol header 'Content-Length: ' mark to get its value. */
    if((key_pos = strstr(head_data, name)) == NULL) {
        strcpy(value, "");
        return;
    }

    if((value_pos = strstr(key_pos+key_len, "\r\n")) == NULL) {
        strcpy(value, "");
        return;
    }

    value_len = value_pos-(key_pos+key_len);
    if(value_len > len) {
        strcpy(value, "");
        return;
    }

    strncpy(value, key_pos+key_len, value_len);
    return;
}


void EIO_HttpParser::form_get(char* form_data, char* name, char* value, int len)
{
    int j = 0;
    char tmp[64+4];
    strcpy(tmp, name);
    strcat(tmp, "=");

    if(form_data == NULL){
        strcpy(value, "");
        return;
    }

    char *p = strstr(form_data, tmp);
    if(p == NULL){
        strcpy(value, "");
        return;
    }

    p += strlen(tmp);
    while(*p && *p != '&') {
        value[j++] = *p;
        if(j>=len)	return;
        p ++;
    }

    value[j] = '\0';
}

