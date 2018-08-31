#include "type.h"
#include "stdlib.h"
#include "stdarg.h"

#ifndef _STD_IO_H_
#define _STD_IO_H_

extern void console_init(int so);
extern void cls();
extern void set_color(uint8 bg, uint8 fg);
extern void print(char* str);
extern void vprintf(const char *format, va_list ap); 
extern void printf(const char *format, ...);

extern void init_serial();
extern char read_serial();
extern void write_serial(char a);

extern void write_port_8(uint16 port,uint8 data);
extern uint8 read_port_8(uint16 port);

#define CONSOLE_BLACK       0
#define CONSOLE_BLUE        1
#define CONSOLE_GREEN       2
#define CONSOLE_CYAN        3
#define CONSOLE_RED         4
#define CONSOLE_MAGENTA     5
#define CONSOLE_BROWN       6
#define CONSOLE_LT_GRAY     7
#define CONSOLE_DK_GRAY     8
#define CONSOLE_LT_BLUE     9
#define CONSOLE_LT_GREEN    10
#define CONSOLE_LT_CYAN     11
#define CONSOLE_LT_RED      12
#define CONSOLE_LD_MAGENTA  13
#define CONSOLE_YELLOW      14
#define CONSOLE_WHITE       15

#endif