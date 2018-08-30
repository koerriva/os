#include "include/asm_macros.h"
#include "include/type.h"
#include "include/stdio.h"

#define COLUMNS                 80
#define LINES                   25
#define VIDEO                   0xB8000

struct Pos {
    int x;
    int y;
} static pos={0,0};

static uint16 color;
static uint16* video;
static int serial_out;

void console_init(int so)
{
    video = (uint16*) VIDEO;
    serial_out = so;
    if (serial_out) {
        init_serial();
    }else{
        cls();
    }
}

void cls(){
    video = (uint16*)VIDEO;
    for(int i=0;i<COLUMNS*LINES;i++){
        *(video++) = 0;
    };
    pos.x=0;
    pos.y=0;
}

void set_color(uint8 bg,uint8 fg){
    color = ((0x0F & bg) << 4 | (0x0F & fg)) << 8;
    color = 0x0a00;
}

static void putchar(char c){
    if(!c)return;
    if (serial_out) {
        write_serial((char) c);
        return;
    }
    video = (uint16*)VIDEO;
    if (c != '\n' && c != '\r') {
        *(video + pos.x + pos.y*COLUMNS) = color + c;
        pos.x++;
    }
    if (pos.x >= COLUMNS || c == '\n' || c == '\r') {
        for(int i=pos.x;i<COLUMNS;i++){
            *(video + pos.x + pos.y*COLUMNS) = 0x0;
        }
        pos.x=0;
        if (pos.y >= LINES) {
            for (int i = 0; i < LINES * COLUMNS; i++) {
                *(video+i) = *(video + i + LINES);
            }
            pos.y--;
            *(video + pos.x + pos.y*COLUMNS) = color + c;
            pos.x++;
        }else{
            pos.y++;
        }
    }
}

void print(char* str){
    int i=0;
    while(str[i]){
        putchar(str[i++]);
    }
}

void vprintf (const char *format, va_list ap){
    unsigned long i;
    char c;
    char buf[20];

    while ((c = *format++) != 0)
    {
        if (c != '%')
        putchar (c);
        else
        {
            char *p;

            c = *format++;
            switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
            case 'p':
                i = va_arg(ap, unsigned long);
                itoa (buf, c, i);
                p = buf;
                goto string;
                break;

            case 's':
                p = va_arg(ap, char *);
                if (! p)
                p = "(null)";

            string:
                while (*p)
                putchar (*p++);
                break;

            default:
                putchar (va_arg(ap, unsigned long));
                break;
            }
        }
    }
}

void printf(const char *format, ...){
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
}

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