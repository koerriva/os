#include "include/lib.h"
#include "include/vga.h"
#include "include/multiboot2.h"
#include "include/console.h"

void kernel_main(uint64 magic,uint64 addr){
    struct multiboot_tag * tag;
    unsigned int size;
    size = *(unsigned int *) addr;
    for (tag = (struct multiboot_tag *) (addr + 8);
            tag->type != MULTIBOOT_TAG_TYPE_END;
            tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))) {
        switch(tag->type){
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
               {
                 multiboot_uint32_t color;
                 unsigned i;
                 struct multiboot_tag_framebuffer *tagfb
                   = (struct multiboot_tag_framebuffer *) tag;
                 void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;
     
                 switch (tagfb->common.framebuffer_type)
                   {
                   case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
                     {
                       unsigned best_distance, distance;
                       struct multiboot_color *palette;
     
                       palette = tagfb->framebuffer_palette;
     
                       color = 0;
                       best_distance = 4*256*256;
     
                       for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++)
                         {
                           distance = (0xff - palette[i].blue)
                             * (0xff - palette[i].blue)
                             + palette[i].red * palette[i].red
                             + palette[i].green * palette[i].green;
                           if (distance < best_distance)
                             {
                               color = i;
                               best_distance = distance;
                             }
                         }
                     }
                     break;
     
                   case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                     color = ((1 << tagfb->framebuffer_blue_mask_size) - 1)
                       << tagfb->framebuffer_blue_field_position;
                     break;
     
                   case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                     color = '\\' | 0x0100;
                     break;
     
                   default:
                     color = 0xffffffff;
                     break;
                   }
     
                 for (i = 0; i < tagfb->common.framebuffer_width
                        && i < tagfb->common.framebuffer_height; i++)
                   {
                     switch (tagfb->common.framebuffer_bpp)
                       {
                       case 8:
                         {
                           multiboot_uint8_t *pixel = fb
                             + tagfb->common.framebuffer_pitch * i + i;
                           *pixel = color;
                         }
                         break;
                       case 15:
                       case 16:
                         {
                           multiboot_uint16_t *pixel
                             = fb + tagfb->common.framebuffer_pitch * i + 2 * i;
                           *pixel = color;
                         }
                         break;
                       case 24:
                         {
                           multiboot_uint32_t *pixel
                             = fb + tagfb->common.framebuffer_pitch * i + 3 * i;
                           *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
                         }
                         break;
     
                       case 32:
                         {
                           multiboot_uint32_t *pixel
                             = fb + tagfb->common.framebuffer_pitch * i + 4 * i;
                           *pixel = color;
                         }
                         break;
                       }
                   }
                 break;
               }
            default:break;
        }
    }
}