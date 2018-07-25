#include "type.h"
extern void write_port_8(uint16 port,uint8 data);
extern uint8 read_port_8(uint16 port);

#define outport     write_port_8
#define inport      read_port_8