#ifndef __EIO_H_INCLUDED__
#define __EIO_H_INCLUDED__

#define EIO_VERSION  1.2.0

#include <iostream>
using namespace std;

#include <ext/hash_map>
using namespace __gnu_cxx;


/*
 * define data type
 */
typedef  int        BOOL;
typedef  int16_t    int16;
typedef  uint32_t   uint32;

#define  TRUE   1
#define  FALSE  0

#define KB(n)  (n*1024)
#define MB(n)  (n*1024*1024)

#define MIN(A,B) (A)<(B)?(A):(B)
#define MAX(A,B) (A)>(B)?(A):(B)

#endif
