#include "include/type.h"

extern void print(char* str);

void print(char* str){
    uint16* addr = (uint16*)0xB8000;
    int i=-1;
    while(str[++i]!=0){
        *(addr++) = str[i]+0x2a00;
    }
}