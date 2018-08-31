#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

typedef long unsigned size_t;
typedef unsigned long uintptr_t;
typedef long intptr_t;

#define FILE int
#define NULL 0
#define stderr ((int *) 1)
#define stdout ((int *) 0)

typedef long __time_t;
typedef __time_t time_t;
struct timespec {
    __time_t tv_sec;
    __time_t tv_nsec;
};

typedef __time_t suseconds_t;
struct timeval {
   time_t      tv_sec;     /* seconds */
   suseconds_t tv_usec;    /* microseconds */
};

struct timezone {
    int tz_minuteswest;     /* minutes west of Greenwich */
    int tz_dsttime;         /* type of DST correction */
};

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int int16_t;
typedef unsigned short int uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long       int64_t;
typedef unsigned long  uint64_t;

#define INT_MAX 2147483647
#define LONG_MAX 9223372036854775807L
#define INT_MIN (-2147483647 - 1)
#define LONG_MIN (-9223372036854775807LL -1)

#define SIZEOF_VOID_P 4

#endif