#include "include/vbe.h"
#include "include/multiboot2.h"
#include "include/video.h"

void init_video(void* boot_info_addr,struct VideoInfo *videoInfo) {
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
            
            videoInfo->addr = (uint64)fb;
            videoInfo->width = tagfb->common.framebuffer_width;
            videoInfo->height = tagfb->common.framebuffer_height;
            videoInfo->bits = tagfb->common.framebuffer_bpp;
            videoInfo->pitch = tagfb->common.framebuffer_pitch;
            videoInfo->type = tagfb->common.framebuffer_type;
        }
    }
}