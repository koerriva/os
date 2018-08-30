#include "include/vbe.h"
#include "include/multiboot2.h"
#include "include/video.h"

void init_video(void* boot_info_addr,struct VideoInfo *videoInfo) {
    printf("Boot Info Addr : 0x%x\n",boot_info_addr);
    struct multiboot_tag * tag;
    unsigned int size;
    size = *(unsigned int *) boot_info_addr;
    for (tag = (struct multiboot_tag *) (boot_info_addr + 8);
            tag->type != MULTIBOOT_TAG_TYPE_END;
            tag = (struct multiboot_tag *) ((uint8 *) tag + ((tag->size + 7) & ~7))) {
        if(tag->type==MULTIBOOT_TAG_TYPE_FRAMEBUFFER){
            struct multiboot_tag_framebuffer *tagfb
                = (struct multiboot_tag_framebuffer *) tag;
                void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;
            uint32 width = tagfb->common.framebuffer_width;
            uint32 height = tagfb->common.framebuffer_height;
            uint8  bits = tagfb->common.framebuffer_bpp;
            uint32 pitch = tagfb->common.framebuffer_pitch;
            uint8  type = tagfb->common.framebuffer_type;
            
            printf("Video Addr      :0x%x\n",fb);
            printf("Video Width     :%d\n",width);
            printf("Video Height    :%d\n",height);
            printf("Video Bpp       :%d\n",bits);
            printf("Video Pitch     :%d\n",pitch);
            printf("Vidoe Type      :%d\n",type);
            videoInfo->addr = fb;
            videoInfo->width = width;
            videoInfo->height = height;
            videoInfo->bits = bits;
            videoInfo->pitch = pitch;
            videoInfo->type = type;
        }
    }
}