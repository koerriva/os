#include "type.h"
extern void write_port_8(uint16 port,uint8 data);
extern uint8 read_port_8(uint16 port);
extern long _vm86(void *code,size_t size);
//libc
extern void* mem_copy(void *dst, const void *src,size_t size);

#define outport     write_port_8
#define inport      read_port_8