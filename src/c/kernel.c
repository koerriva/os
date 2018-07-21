#include "include/lib.h"
#include "include/vga.h"

void set_mode(void);
void put_pixel(UINT32 x,UINT32 y,UINT8 r, UINT8 g, UINT8 b);

void kernel_main(){
    write_mem(0xb8000,0x2a592b412c4b2d4f);
    set_mode();
    for(UINT32 x=1;x<100;x++){
        for(UINT32 y=1;y<100;y++){
            put_pixel(x,y,100,10,10); 
        }
    }
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
        outport(GRAPHICS_CONTROLLER_DATA_PORT,*(regs));
    }

    //attribute controller
    for(char i=0;i<21;i++){
        inport(ATTRIBUTE_CONTROLLER_RESET_PORT);
        outport(ATTRIBUTE_CONTROLLER_INDEX_PORT,i);
        outport(ATTRIBUTE_CONTROLLER_WRITE_PORT,*(regs++));
    }

    inport(ATTRIBUTE_CONTROLLER_RESET_PORT);
    outport(ATTRIBUTE_CONTROLLER_WRITE_PORT,0x20);
}

void set_mode(){
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

    write_regs(g_320x200x256);
}

UINT8* get_frame_buffer_segment(){
    outport(GRAPHICS_CONTROLLER_INDEX_PORT,0x06);
    UINT8 segment_num = inport(GRAPHICS_CONTROLLER_DATA_PORT) & (3<<2);
    switch(segment_num){
        default:
        case 0<<2: return (UINT8*)0x00000;
        case 1<<2: return (UINT8*)0xA0000;
        case 2<<2: return (UINT8*)0xB0000;
        case 3<<2: return (UINT8*)0xB8000;
    }
}

UINT8 get_color_index(UINT8 r, UINT8 g, UINT8 b)
{
    if(r == 0x00 && g == 0x00 && b == 0x00) return 0x00; // black
    if(r == 0x00 && g == 0x00 && b == 0xA8) return 0x01; // blue
    if(r == 0x00 && g == 0xA8 && b == 0x00) return 0x02; // green
    if(r == 0xA8 && g == 0x00 && b == 0x00) return 0x04; // red
    if(r == 0xFF && g == 0xFF && b == 0xFF) return 0x3F; // white
    return 0x00;
}

void put_pixel(UINT32 x,UINT32 y,UINT8 r, UINT8 g, UINT8 b){
    if((x<0||320<=x) || (y<0||200<=y))return;
    UINT8* pixelAddr = get_frame_buffer_segment()+320*y+x;
    *pixelAddr = get_color_index(r,g,b);
}