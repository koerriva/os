#include "include/type.h"
#include "include/print.h"

enum uint16TextColor{
    Black      = 0,
    Blue       = 1,
    Green      = 2,
    Cyan       = 3,
    Red        = 4,
    Magenta    = 5,
    Brown      = 6,
    LightGray  = 7,
    DarkGray   = 8,
    LightBlue  = 9,
    LightGreen = 10, //0xa
    LightCyan  = 11, //0xb
    LightRed   = 12, //0xc
    Pink       = 13, //0xd
    Yellow     = 14, //0xe
    White      = 15 //0xf
};

struct TextPos {
    int x;
    int y;
} pos={0,0};

extern void print(char* str,uint16 color);
extern void printf(char* str,uint16 color,long num);
extern void printfs(char* str,uint16 color,long* params,int size);
extern char* itoa(long num,char* str,int radix);

void print(char* str,uint16 color){
    uint16* addr = (uint16*)TEXT_BUFFER_SEGMENT + pos.y*TEXT_BUFFER_WIDTH + pos.x;
    int i=-1;
    while(str[++i]!=0){
        if(str[i]=='\n'){
            pos.x=0;
            pos.y++;
        }else{
            *(addr+pos.y*TEXT_BUFFER_WIDTH + pos.x) = str[i]+color;
            pos.x++;
        }
    }
}

void printf(char* str,uint16 color,long num){
    char temp[16]={0};
    uint16* addr = (uint16*)TEXT_BUFFER_SEGMENT + pos.y*TEXT_BUFFER_WIDTH + pos.x;
    int i=-1;
    while(str[++i]!=0){
        if(str[i]=='?'){
            print(itoa(num,temp,16),color);
        }else if(str[i]=='\n'){
            pos.x=0;
            pos.y++;
        }else{
            *(addr+pos.y*TEXT_BUFFER_WIDTH + pos.x) = str[i]+color;
            pos.x++;
        }
    }
}

void printfs(char* str,uint16 color,long* params,int size){
    char temp[size][16];
    for(int m=0;m<size;m++){
        for(int n=0;n<16;n++){
            temp[m][n]=0;
        }
    }
    uint16* addr = (uint16*)TEXT_BUFFER_SEGMENT + pos.y*TEXT_BUFFER_WIDTH + pos.x;
    int s = 0;
    for(int i=0;str[i]!=0;i++){
        char c = str[i];
        if(c=='?'){
            print(itoa(params[s],temp[s],16),color);
            s++;
        }else if(c=='\n'){
            pos.y=TEXT_BUFFER_WIDTH-pos.x;
            pos.x=0;
        }else{
            *(addr+pos.y + pos.x) = str[i]+color;
            pos.x++;
        }
    }
}

char* itoa(long num,char* str,int radix)
{   /*索引表*/
    char index[]="0123456789ABCDEF";
    unsigned unum;/*中间变量*/
    int i=0,j,k;
    /*确定unum的值*/
    if(radix==10&&num<0)/*十进制负数*/
    {
        unum=(unsigned)-num;
        str[i++]='-';
    }
    else unum=(unsigned)num;/*其他情况*/
    /*转换*/
    do{
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
    }while(unum);
    str[i]='\0';
    /*逆序*/
    if(str[0]=='-')k=1;/*十进制负数*/
    else k=0;
    char temp;
    for(j=k;j<=(i-1)/2;j++)
    {
        temp=str[j];
        str[j]=str[i-1+k-j];
        str[i-1+k-j]=temp;
    }
    return str;
}