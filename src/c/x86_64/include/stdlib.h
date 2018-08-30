#include "type.h"

#ifndef _STD_LIB_H_
#define _STD_LIB_H_

//libc
extern void *mem_copy(void *d, const void *s, unsigned n);

//utils
extern char* itoa(char* buf,char base,long d);

#define outport     write_port_8
#define inport      read_port_8

#endif