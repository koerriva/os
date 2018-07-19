void write_mem_64(unsigned long addr,unsigned long data);

#define write_mem write_mem_64

void kernel_main(){
    write_mem(0xb8000,0x2a592b412c4b2d4f);
}