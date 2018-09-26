#include "include/video.h"
#include "include/stdio.h"
#include "include/stdlib.h"
#include "include/idt.h"

uint64 address(unsigned int pdpt_idx,unsigned int pt_idx){
    return 0|(pdpt_idx<<30)|(pt_idx<<21);
}

uint64 PhyscAddrToVirtAddr(uint64 physcAddr){
    unsigned int pt_idx = physcAddr / 0x200000U;
    unsigned int offset = physcAddr % 0x200000U;
    printf("pd_idx:%d\n",pt_idx);
    printf("offset :%d\n",offset);
    unsigned int pdpt_idx = 0;
    if(pt_idx>512){
        pdpt_idx = pt_idx / 512;
        pt_idx = pt_idx % 512;
        printf("pd_idx:%d\n",pt_idx);
        printf("pdpt_idx:%d\n",pdpt_idx);
        return address(pdpt_idx,pt_idx);
    }else{
        return physcAddr;
    }
}

uint64 checkVMem(uint64 p){
    uint64 addr = PhyscAddrToVirtAddr(p);
    printf("check vmem : 0x%x\n",addr);
    return addr;
}

extern uint64 gdt64_base;
extern uint64 tss64_base;

void kernel_main(uint64 * pml4,void* addr){
    console_init(1);
    printf("-------------------------\n");
    printf("GTD Addr : 0x%x\nTSS Addr : 0x%x\n",gdt64_base,tss64_base);
    printf("SETUP IDT ...\n");
    setup_idt();
    printf("INIT VIDEO ...\n");
    struct VideoInfo videoInfo;
    init_video(addr,&videoInfo);
    if(!videoInfo.addr){
        printf("INIT FAIL !!!\n");
        return;
    }
    printf("-------------------------\n");
    printf("IDENTITY VIDEO MEMORY ...\n");
    printf("PML4 Addr : 0x%x\n",pml4);
    uint64 * pdpt = (uint64 *)(*pml4 & 0xffffffffffffff00U);
    printf("PDPT Addr : 0x%x\n",pdpt);
    uint64 * pd = (uint64 *)(*pdpt & 0xffffffffffffff00U);
    printf("PD   Addr : 0x%x\n",pd);

    for(int y=5;y<13;y++){
        *(pd+y) = (y-5)*0x200000U + videoInfo.addr | 0x83U;
        printf("VM:0x%x MM:0x%x\n",*(pd+y),y*0x200000U);
    }

    uint32* vram = (uint32 *)(5*0x200000U);
    printf("VIDEO MEMORY START : 0x%x,0x%x\n",vram,address(0,5));
    for(int i=0;i<videoInfo.width*videoInfo.height;i++){
        *(vram++) = 0x00FFFFAF;
    }

    uint64* ptr = (uint64 *)0xafeeeeeeeeeee;
    *ptr = 0;
    
    printf("i'm not crash!!!\n");
    while(1);
}

static struct IDTDescr IDT[IDT_COUNT];
static struct IDTPtr IDTPtr;
static uint64_t IDTHandler[IDT_COUNT];
void init_handler(){
    uint64_t addr;
    for(int i=0;i<IDT_COUNT;i++){
        switch(i){
            case 0x0:addr = (uint64_t)_int_0x0_handler;break;
            case 0x1:addr = (uint64_t)_int_0x1_handler;break;
            case 0x3:addr = (uint64_t)_int_0x3_handler;break;
            case 0x8:addr = (uint64_t)_int_0x8_handler;break;
            case 0xd:addr = (uint64_t)_int_0xd_handler;break;
            case 0xe:addr = (uint64_t)_int_0xe_handler;break;
            default:addr = (uint64_t)_int_xx_handler;
        }
        IDTHandler[i] = addr;
    }
}
void setup_idt(void){
    init_handler();
    uint16 cs = (uint16)get_cs();
    uint64 addr;
    const int count = IDT_COUNT;
    for(int i=0;i<count;i++){
        addr = IDTHandler[i];
        struct IDTDescr idtDescr;
        idtDescr.offset_1 = L16(addr);
        idtDescr.selector = cs;
        idtDescr.ist = 0x0;
        idtDescr.type_attr = 0b11101110;
        idtDescr.offset_2 = M16(addr);
        idtDescr.offset_3 = H32(addr);
        idtDescr.zero = 0x0;
        IDT[i] = idtDescr;
    }
    uint16 limit = sizeof(IDT);
    IDTPtr.base_addr = (uint64_t)IDT;
    IDTPtr.limit = limit-1;
    printf("addr:0x%x, limit:%d, ptr：0x%x\n",IDT,limit,&IDTPtr);
    load_idt((uint64_t)(&IDTPtr));
}