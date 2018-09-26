#include "type.h"
#ifndef _IDT_H_
#define _IDT_H_
#pragma pack(1)
struct IDTDescr {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t zero;
};
struct IDTPtr {
    uint16_t limit;
    uint64_t base_addr;
};
struct ExceptionStackFrame{
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};
#pragma pack()

#define IDT_COUNT 256

extern void ignore_init();
extern void load_idt(uint64);
extern uint64 get_cs();
void setup_idt();
#define L16(v) ((uint16_t)v)
#define M16(v) ((uint16_t)(v>>16))
#define H32(v) ((uint32_t)(v>>32))

extern void _int_xx_handler();
extern void _int_0x0_handler();
extern void _int_0x1_handler();
extern void _int_0x3_handler();
extern void _int_0x8_handler();
extern void _int_0xd_handler();
extern void _int_0xe_handler();
#endif