#ifndef __EIO_HTTP_PARSER_H_INCLUDED__
#define __EIO_HTTP_PARSER_H_INCLUDED__


class EIO_HttpParser
{
public:
    static int head_len(char* http_data);
    static void uri_get(const char* head_data, char* method, char* uri, char* version);
    static void head_get(char* head_data, char* name, char* value, int len);
    static void form_get(char* form_data, char* name, char* value, int len);
};

#endif

