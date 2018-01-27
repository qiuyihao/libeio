#ifndef __EIO_ICONV_H_INCLUDED__
#define __EIO_ICONV_H_INCLUDED__

#include <stdint.h>


class EIO_Iconv
{
public:
    static void dump_utf8(char* utf8);

    static void Utf8ToUnicode(wchar_t* pOut, char *pText);
    static void UnicodeToUtf8(char* pOut, wchar_t* pText);
    static int  UnicodeToUtf8(uint16_t* in, int insize, uint8_t* out);
    static int  Utf16ToUtf8(unsigned char *out, int *outlen, const unsigned char *inb, int *inlenb);
};

#endif

