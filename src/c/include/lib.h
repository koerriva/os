#include "type.h"
void write_mem_64(UINT64 addr,UINT64 data);
void sleep(void);
void write_port_8(UINT32 port,UINT8 data);
UINT8 read_port_8(UINT16 port);

#define write_mem write_mem_64
#define outport write_port_8
#define inport read_port_8