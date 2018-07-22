#include "include/lib.h"
#include "include/vga.h"

void set_mode(int idx);
void put_pixel(uint32 x,uint32 y,uint8 r, uint8 g, uint8 b);
void put_pixel_(uint32 x,uint32 y,uint8 color_index);

void kernel_main(){
    print("Welcome My OS !");
    set_mode(1);
    while(1);
}

void write_regs(unsigned char* regs){
    // misc
    outport(MISC_PORT,*(regs++));

    //sequencer
    for(char i=0;i<5;i++){
        outport(SEQUENCER_INDEX_PORT,i);
        outport(SEQUENCER_DATA_PORT,*(regs++));
    }

    //cathoder ray tube controller
    outport(CRTC_INDEX_PORT,0x03);
    outport(CRTC_DATA_PORT,inport(CRTC_DATA_PORT)|0x80);
    outport(CRTC_INDEX_PORT,0x11);
    outport(CRTC_DATA_PORT,inport(CRTC_DATA_PORT) & ~0x80);

    regs[0x03] = regs[0x03] | 0x80;
    regs[0x11] = regs[0x11] & ~0x80;

    for(char i=0;i<25;i++){
        outport(CRTC_INDEX_PORT,i);
        outport(CRTC_DATA_PORT,*(regs++));
    }

    //graphics controller
    for(char i=0;i<9;i++){
        outport(GRAPHICS_CONTROLLER_INDEX_PORT,i);
        outport(GRAPHICS_CONTROLLER_DATA_PORT,*(regs++));
    }

    //attribute controller
    for(char i=0;i<21;i++){
        inport(ATTRIBUTE_CONTROLLER_RESET_PORT);
        outport(ATTRIBUTE_CONTROLLER_INDEX_PORT,i);
        outport(ATTRIBUTE_CONTROLLER_WRITE_PORT,*(regs++));
    }

    inport(ATTRIBUTE_CONTROLLER_RESET_PORT);
    outport(ATTRIBUTE_CONTROLLER_INDEX_PORT,0x20);
}

void set_mode(int idx){
    unsigned char g_320x200x256[] =
    {
        /* MISC */
            0x63,
        /* SEQ */
            0x03, 0x01, 0x0F, 0x00, 0x0E,
        /* CRTC */
            0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
            0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
            0xFF,
        /* GC */
            0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
            0xFF,
        /* AC */
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x41, 0x00, 0x0F, 0x00, 0x00
    };
    unsigned char g_720x480x16[] =
    {
    /* MISC */
        0xE7,
    /* SEQ */
        0x03, 0x01, 0x08, 0x00, 0x06,
    /* CRTC */
        0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
        0x00, 0x40, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
        0xEA, 0x0C, 0xDF, 0x2D, 0x08, 0xE8, 0x05, 0xE3,
        0xFF,
    /* GC */
        0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
        0xFF,
    /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x01, 0x00, 0x0F, 0x00, 0x00,
    };

    write_regs(g_320x200x256);

    for(uint32 y=0;y<VGA_HEIGHT;y++){
        for(uint32 x=0;x<VGA_WIDTH;x++){
            put_pixel(x,y,0xA8,0x0,0x0);
        }
    }
}

uint8* get_frame_buffer_segment(){
    outport(GRAPHICS_CONTROLLER_INDEX_PORT,0x06);
    uint8 segment_num = inport(GRAPHICS_CONTROLLER_DATA_PORT) & (3<<2);
    switch(segment_num){
        default:
        case 0<<2: return (uint8*)0x00000;
        case 1<<2: return (uint8*)0xA0000;
        case 2<<2: return (uint8*)0xB0000;
        case 3<<2: return (uint8*)0xB8000;
    }
}

uint8 get_color_index(uint8 r, uint8 g, uint8 b)
{
    if(r == 0x00 && g == 0x00 && b == 0x00) return 0x00; // black
    if(r == 0x00 && g == 0x00 && b == 0xA8) return 0x01; // blue
    if(r == 0x00 && g == 0xA8 && b == 0x00) return 0x02; // green
    if(r == 0xA8 && g == 0x00 && b == 0x00) return 0x04; // red
    if(r == 0xFF && g == 0xFF && b == 0xFF) return 0x3F; // white
    return 0x00;
}

void put_pixel(uint32 x,uint32 y,uint8 r, uint8 g, uint8 b){
    if((x<0||VGA_WIDTH<=x) || (y<0||VGA_HEIGHT<=y))return;
    uint8* pixelAddr = get_frame_buffer_segment()+VGA_WIDTH*y+x;
    *pixelAddr = get_color_index(r,g,b);
}

void put_pixel_c(uint32 x,uint32 y,uint8 color_index){
    if((x<0||VGA_WIDTH<=x) || (y<0||VGA_HEIGHT<=y))return;
    uint8* pixelAddr = get_frame_buffer_segment()+VGA_WIDTH*y+x;
    *pixelAddr = color_index;
}