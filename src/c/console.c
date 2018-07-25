#include "include/type.h"
#include "include/console.h"
#include "include/string.h"

#define COLUMNS                 80
#define LINES                   25
#define VIDEO                   0xB8000

struct Pos {
    int x;
    int y;
} static pos={0,0};

static uint16 color;
static uint16* video;

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
    video = (uint16*)VIDEO;
    if(c==0)return;
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
    int i=-1;
    while(str[++i]!=0){
        putchar(str[i]);
    }
}

void vprintf (const char *format, va_list ap){
    int i;
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
                i = va_arg(ap, int);
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
                putchar (va_arg(ap, int));
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