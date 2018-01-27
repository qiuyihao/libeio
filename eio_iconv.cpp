/**
 *
    +----+-----+----+-----+----+-----+----+-----+
    | Hx | Chr | Hx | Chr | Hx | Chr | Hx | Chr |
    +----+-----+----+-----+----+-----+----+-----+
    | 00 | NUL | 20 | SPC | 40 |  @  | 60 |  `  |
    | 01 | SOH | 21 |  !  | 41 |  A  | 61 |  a  |
    | 02 | STX | 22 |  "  | 42 |  B  | 62 |  b  |
    | 03 | ETX | 23 |  #  | 43 |  C  | 63 |  c  |
    | 04 | EOT | 24 |  $  | 44 |  D  | 64 |  d  |
    | 05 | ENQ | 25 |  %  | 45 |  E  | 65 |  e  |
    | 06 | ACK | 26 |  &  | 46 |  F  | 66 |  f  |
    | 07 | BEL | 27 |  '  | 47 |  G  | 67 |  g  |
    | 08 | BS  | 28 |  (  | 48 |  H  | 68 |  h  |
    | 09 | TAB | 29 |  )  | 49 |  I  | 69 |  i  |
    | 0A | LF  | 2A |  *  | 4A |  J  | 6A |  j  |
    | 0B | VT  | 2B |  +  | 4B |  K  | 6B |  k  |
    | 0C | FF  | 2C |  ,  | 4C |  L  | 6C |  l  |
    | 0D | CR  | 2D |  -  | 4D |  M  | 6D |  m  |
    | 0E | SO  | 2E |  .  | 4E |  N  | 6E |  n  |
    | 0F | SI  | 2F |  /  | 4F |  O  | 6F |  o  |
    | 10 | DLE | 30 |  0  | 50 |  P  | 70 |  p  |
    | 11 | DC1 | 31 |  1  | 51 |  Q  | 71 |  q  |
    | 12 | DC2 | 32 |  2  | 52 |  R  | 72 |  r  |
    | 13 | DC3 | 33 |  3  | 53 |  S  | 73 |  s  |
    | 14 | DC4 | 34 |  4  | 54 |  T  | 74 |  t  |
    | 15 | NAK | 35 |  5  | 55 |  U  | 75 |  u  |
    | 16 | SYN | 36 |  6  | 56 |  V  | 76 |  v  |
    | 17 | ETB | 37 |  7  | 57 |  W  | 77 |  w  |
    | 18 | CAN | 38 |  8  | 58 |  X  | 78 |  x  |
    | 19 | EM  | 39 |  9  | 59 |  Y  | 79 |  y  |
    | 1A | SUB | 3A |  :  | 5A |  Z  | 7A |  z  |
    | 1B | ESC | 3B |  ;  | 5B |  [  | 7B |  {  |
    | 1C | FS  | 3C |  <  | 5C |  \  | 7C |  |  |
    | 1D | GS  | 3D |  =  | 5D |  ]  | 7D |  }  |
    | 1E | RS  | 3E |  >  | 5E |  ^  | 7E |  ~  |
    | 1F | US  | 3F |  ?  | 5F |  _  | 7F | DEL |
    +----+-----+----+-----+----+-----+----+-----+
 *
 */

#include "eio_iconv.h"
#include <stdio.h>
#include <iostream>
using namespace std;


void EIO_Iconv::dump_utf8(char* utf8)
{
    char* p = utf8;

    while(*p)
    {
        fprintf(stderr, "%02X", *p);
        p ++;
    }
    putchar('\n');
}


void EIO_Iconv::Utf8ToUnicode(wchar_t* pOut, char *pText)
{
    char* uchar = (char *)pOut;
    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
    return;
}


void EIO_Iconv::UnicodeToUtf8(char* pOut, wchar_t* pText)
{
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    return;
}


int EIO_Iconv::UnicodeToUtf8(uint16_t* in, int insize, uint8_t* out)
{
    int charscount = insize / 2;
    int outsize = 0;
    uint8_t* tmp = out;

    for(int i = 0; i < charscount; i ++)
    {
        uint16_t unicode = in[i];
        uint8_t* unicode_ptr = (uint8_t*)&unicode;

        if (unicode >= 0x0000 && unicode <= 0x007f)
        {
            *tmp = (uint8_t)unicode;
            tmp += 1;
            outsize += 1;
        }
        else if (unicode >= 0x0080 && unicode <= 0x07ff)
        {
            *tmp = 0xc0 | (unicode >> 6);
            tmp += 1;
            *tmp = 0x80 | (unicode & (0xff >> 2));
            tmp += 1;
            outsize += 2;
        }
        else if (unicode >= 0x0800 && unicode <= 0xffff)
        {
/*
            *tmp = 0xe0 | (unicode >> 12);
            tmp += 1;
            *tmp = 0x80 | (unicode >> 6 & 0x00ff);
            tmp += 1;
            *tmp = 0x80 | (unicode & (0xff >> 2));
            tmp += 1;
            outsize += 3;
*/

            *tmp = (0xE0 | ((unicode_ptr[1] & 0xF0) >> 4));
            tmp += 1;
            *tmp = (0x80 | ((unicode_ptr[1] & 0x0F) << 2)) + ((unicode_ptr[0] & 0xC0) >> 6);
            tmp += 1;
            *tmp = (0x80 | (unicode_ptr[0] & 0x3F));
            tmp += 1;
            outsize += 3;
        }
    }

    *tmp = '\0';
    return outsize;
}


int EIO_Iconv::Utf16ToUtf8(unsigned char *out, int *outlen, const unsigned char *inb, int *inlenb)
{
    unsigned char* outstart = out;
    const unsigned char* processed = inb;
    unsigned char* outend = out + *outlen;
    unsigned short* in = (unsigned short*) inb;
    unsigned short* inend;
    unsigned int c, d, inlen;
    unsigned char *tmp;
    int bits;
    
    int isLittleEndian = 1;

    if((*inlenb % 2) == 1) (*inlenb)--;
    inlen = *inlenb / 2;
    inend = in + inlen;
    while((in < inend) && (out - outstart + 5 < *outlen))
    {
        if(isLittleEndian)
        {
            c= *in++;
        }
        else
        {
            tmp = (unsigned char *) in;
            c   = *tmp++;
            c   = c | (((unsigned int)*tmp) << 8);
            in++;
        }
        if((c & 0xFC00) == 0xD800)
        {
            if(in >= inend) break;
            if(isLittleEndian) { d = *in++; }
            else
            {
                tmp = (unsigned char *) in;
                d   = *tmp++;
                d   = d | (((unsigned int)*tmp) << 8);
                in++;
            }
            if((d & 0xFC00) == 0xDC00)
            {
                c &= 0x03FF;
                c <<= 10;
                c |= d & 0x03FF;
                c += 0x10000;
            }
            else
            {
                *outlen = out - outstart;
                *inlenb = processed - inb;
                return -1;
            }
        }

        if(out >= outend)    break;
        if     (c <    0x80) { *out++ =  c;                        bits= -6; }
        else if(c <   0x800) { *out++ = ((c >>  6) & 0x1F) | 0xC0; bits=  0; }
        else if(c < 0x10000) { *out++ = ((c >> 12) & 0x0F) | 0xE0; bits=  6; }
        else                 { *out++ = ((c >> 18) & 0x07) | 0xF0; bits= 12; }

        for(; bits >= 0; bits-= 6)
        {
            if (out >= outend)
            break;
            *out++ = ((c >> bits) & 0x3F) | 0x80;
        }
        processed = (const unsigned char*) in;
    }

    *outlen = out - outstart;
    *inlenb = processed - inb;

    return(*outlen);
}

