#include "include/video.h"
#include "include/stdio.h"
#include "include/stdlib.h"

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

void kernel_main(uint64 * pml4,void* addr){
    console_init(1);
    printf("-------------------------\n");
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
    while(1);
}