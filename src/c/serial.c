
#include "include/asm_macros.h"


#define PORT 0x3f8   /* COM1 */
 
void init_serial() {
    ASM_OUTPORT_B(PORT + 1, 0x00);    // Disable all interrupts
    ASM_OUTPORT_B(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    ASM_OUTPORT_B(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    ASM_OUTPORT_B(PORT + 1, 0x00);    //                  (hi byte)
    ASM_OUTPORT_B(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    ASM_OUTPORT_B(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    ASM_OUTPORT_B(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}


static int serial_received() {
    char t;
    ASM_INPORT_B(PORT + 5, t);
    return t & 1;
}
    
char read_serial() {
    char t;

    while (serial_received() == 0);

    ASM_INPORT_B(PORT, t);
    return t;
}


static int is_transmit_empty() {
    char t;
    ASM_INPORT_B(PORT + 5, t);
    return t & 0x20;
}
    
void write_serial(char a) {
    while (is_transmit_empty() == 0); 

    ASM_OUTPORT_B(PORT,a);
}
