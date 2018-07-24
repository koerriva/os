#include "type.h"
void write_port_8(uint16 port,uint8 data);
uint8 read_port_8(uint16 port);

#define outport     write_port_8
#define inport      read_port_8

void print(char* str,uint16 color);
void printf(char* str,uint16 color,long num);
char* itoa(int num,char* str,int radix);