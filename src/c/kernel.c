#include "include/lib.h"
#include "include/vga.h"
#include "include/multiboot2.h"
#include "include/console.h"

void kernel_main(uint64 magic,uint64 addr){
    console_init(1);
    printf("-------------------------\n");
    printf("Magic Num : 0x%x\n",magic);
    printf("Boot Info Addr : 0x%x\n",addr);
    struct multiboot_tag * tag;
    unsigned int size;
    size = *(unsigned int *) addr;
    for (tag = (struct multiboot_tag *) (addr + 8);
            tag->type != MULTIBOOT_TAG_TYPE_END;
            tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))) {
        switch(tag->type){
            case MULTIBOOT_TAG_TYPE_VBE: {
                struct multiboot_tag_vbe *tagvbe
                = (struct multiboot_tag_vbe *) tag;
                printf("VBE MODE    :0x%x\n",tagvbe->vbe_mode);
                printf("VBE SEG     :0x%x\n",tagvbe->vbe_interface_seg);
                printf("VBE OFF     :0x%x\n",tagvbe->vbe_interface_off);
                printf("VBE LEN     :0x%x\n",tagvbe->vbe_interface_len);
                printf("VbeInfoBlock SIZE :%d\n",sizeof(struct VbeInfoBlock));
                printf("ModeInfoBlock SIZE :%d\n",sizeof(struct ModeInfoBlock));

                struct VbeInfoBlock *vb = (struct VbeInfoBlock *)tagvbe->vbe_control_info.external_specification;
                printf("VbeSignature：%s\n",vb->VbeSignature);
                printf("VbeVersion:0x%x\n",vb->VbeVersion);
                printf("Capabilities:0x%x,0x%x,0x%x,0x%x\n"
                    ,vb->Capabilities[0]
                    ,vb->Capabilities[1]
                    ,vb->Capabilities[2]
                    ,vb->Capabilities[3]);
                printf("TotalMemory:%d\n",vb->TotalMemory);

                struct ModeInfoBlock * mb = (struct ModeInfoBlock *)tagvbe->vbe_mode_info.external_specification;
                printf("ModeAttributs:0x%x\n",mb->ModeAttributes);
                printf("ASegment:0x%x,BSegment:0x%x\n",mb->WinASegment,mb->WinBSegment);
                printf("BytesPerScanLine:%d\n",mb->BytesPerScanLine);
                printf("WinGranularity:%dKB\n",mb->WinGranularity);
                printf("WinSize:%dKB\n",mb->WinSize);
                printf("WinFuncPtr:0x%p\n",mb->WinFuncPtr);
                printf("Res:%dx%dx%d\n",mb->XResolution,mb->YResolution,mb->BitsPerPixel);
                printf("Banks:%d,Planes:%d\n",mb->NumberOfBanks,mb->NumberOfPlanes);
                printf("ImagePages:%d\n",mb->NumberOfImagePages);
                printf("BankSize:%dKB,MemoryModel:%d\n",mb->BankSize,mb->MemoryModel);
                print("2.0\n");
                printf("PhysBasePtr:0x%x\n",mb->PhysBasePtr);
                printf("Reserved2:0x%x\n",mb->Reserved2);
                printf("Reserved3:%d\n",mb->Reserved3);
                print("3.0\n");
                printf("LinBytesPerScanLine:%d\n",mb->LinBytesPerScanLine);
                printf("BankNumberOfImagePages:%d\n",mb->BankNumberOfImagePages);
                printf("LinNumberOfImagePages:%d\n",mb->LinNumberOfImagePages);
                printf("MaxClock:%dHz\n",mb->MaxPixelClock);
                break;
            }
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
                multiboot_uint32_t color;
                unsigned i;
                struct multiboot_tag_framebuffer *tagfb
                = (struct multiboot_tag_framebuffer *) tag;
                void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;
    
                switch (tagfb->common.framebuffer_type) {
                    case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED: {
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
                        color = ((1 << tagfb->framebuffer_blue_mask_size) - 1) << tagfb->framebuffer_blue_field_position;
                        break;
        
                    case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                        color = '.' | 0xf000;
                        break;
        
                    default:
                        color = 0xffffffff;
                        break;
                }

                uint32 width = tagfb->common.framebuffer_width;
                uint32 height = tagfb->common.framebuffer_height;
                uint8  bits = tagfb->common.framebuffer_bpp;
                uint32 pitch = tagfb->common.framebuffer_pitch;
                
                // printf("Video Addr      :0x%x\n",fb);
                // printf("Video Width     :%d\n",width);
                // printf("Video Height    :%d\n",height);
                // printf("Video Bpp       :%d\n",bits);
                // printf("Video Pitch     :%d\n",pitch);
                // printf("Vidoe Type      :%d\n",tagfb->common.framebuffer_type);

                multiboot_uint16_t *font = (multiboot_uint16_t *)fb;
                multiboot_uint32_t *pixel = (multiboot_uint32_t *)(0xa0000);
                for (i = 0; i < width; i++){
                    switch (bits) {
                        case 16:
                            *(font++) = color;
                            break;
                        case 32:
                            for(int j=0;j<height;j++){
                                *(pixel++) = color;
                            }
                            break;
                        default:break;
                    }
                }
                break;
            }
            default:break;
        }
    }
}