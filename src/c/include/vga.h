#include "type.h"

#define MISC_PORT 0x3c2
#define CRTC_INDEX_PORT 0x3d4
#define CRTC_DATA_PORT 0x3d5
#define SEQUENCER_INDEX_PORT 0x3c4
#define SEQUENCER_DATA_PORT 0x3c5
#define GRAPHICS_CONTROLLER_INDEX_PORT 0x3ce
#define GRAPHICS_CONTROLLER_DATA_PORT 0x3cf
#define ATTRIBUTE_CONTROLLER_INDEX_PORT 0x3c0
#define ATTRIBUTE_CONTROLLER_READ_PORT 0x3c1
#define ATTRIBUTE_CONTROLLER_WRITE_PORT 0x3c0
#define ATTRIBUTE_CONTROLLER_RESET_PORT 0x3da

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

#define VGA_WIDTH   320
#define VGA_HEIGHT  200

extern void set_mode(int idx);
extern void put_pixel_rgb(uint32 x,uint32 y,uint8 r, uint8 g, uint8 b);
extern void put_pixel_color(uint32 x,uint32 y,uint8 color_index);

#pragma pack (1)
struct VbeInfoBlock {
    uint8_t  VbeSignature[4];    // == "VESA" 4
    uint16_t VbeVersion;     // == 0x0300 for VBE 3.0 2
    uint32_t OemStringPtr;   // VbeFarPtr to OEM String 4
    uint8_t  Capabilities[4]; // 4
    uint32_t VideoModePtr;   // VbeFarPtr to VideoModeList 4
    uint16_t TotalMemory;    // Number of 64Kb memory blocks 2 //20

    uint16_t OemSoftwareRev; // VBE implementation Software revision 2
    uint32_t OemVendorNamePtr;//VbeFarPtr to Vendor Name String; 4
    uint32_t OemProductNamePtr; //4
    uint32_t OemProductRevPtr; //4 //14
    uint8_t  Reserved[222];

    uint8_t  OemData[256];   //OEM String 
};
struct ModeInfoBlock {
    uint16_t ModeAttributes;    // mode attributes
    uint8_t WinAAttributes; //window A attributes
    uint8_t WinBAttributes;  //window B attributes
    uint16_t WinGranularity; //window granularity
    uint16_t WinSize;        //window size
    uint16_t WinASegment; //window A start segment
    uint16_t WinBSegment; //window B start segment
    uint32_t WinFuncPtr;    //real mode pointer to window function
    uint16_t BytesPerScanLine; //bytes per scan line
 //1.2   
    uint16_t XResolution; //horizontal resolution in pixels or characters 3
    uint16_t YResolution; //vertical resolution in pixels or characters
    uint8_t XCharSize;  //character cell width in pixels
    uint8_t YCharSize;  //character cell height in pixels
    uint8_t NumberOfPlanes; //number of memory planes
    uint8_t BitsPerPixel; //bits per pixel
    uint8_t NumberOfBanks; //number of banks
    uint8_t MemoryModel;    //memory model type
    uint8_t BankSize;       //bank size in KB
    uint8_t NumberOfImagePages;//number of images
    uint8_t Reserved1;       //reserved for page function

    uint8_t RedMaskSize; //size of direct color red mask in bits
    uint8_t RedFieldPosition; //bit position of lsb of red mask
    uint8_t GreenMaskSize; //size of direct color green mask in bits
    uint8_t GreenFieldPosition; //bit position of lsb of green mask
    uint8_t BlueMaskSize; //size of direct color blue mask in bits
    uint8_t BlueFieldPosition; //bit position of lsb of blue mask
    uint8_t RsvdMaskSize; //size of direct color reserved mask in bits
    uint8_t RsvdFieldPosition; //bit position of lsb of reserved mask
    uint8_t DirectColorModeInfo; //direct color mode attributes
//VBE2.0
    uint32_t PhysBasePtr;   //physical address for flat memory frame buffer
    uint32_t Reserved2;      //Reserved - always set to 0
    uint16_t Reserved3;      //Reserved - always set to 0
//VBE3.0
    uint16_t LinBytesPerScanLine; //bytes per scan line for linear modes
    uint8_t  BankNumberOfImagePages; //number of images for banked modes
    uint8_t LinNumberOfImagePages; //number of images for linear modes
    uint8_t LinRedMaskSize; //size of direct color red mask (linear modes)
    uint8_t LinRedFieldPosition; //bit position of lsb of red mask (linear modes)
    uint8_t LinGreenMaskSize; //size of direct color green mask (linear modes)
    uint8_t LinGreenFieldPosition; //bit position of lsb of green mask (linear modes)
    uint8_t LinBlueMaskSize; //size of direct color blue mask (linear modes)
    uint8_t LinBlueFieldPosition; //bit position of lsb of blue mask (linear modes)
    uint8_t LinRsvdMaskSize; //size of direct color reserved mask (linear modes)
    uint8_t LinRsvdFieldPosition; //bit position of lsb of reserved mask (linear modes)
    uint32_t MaxPixelClock; //maximum pixel clock (in Hz) for graphics mode

    uint8_t Reserved4[190]; //remainder of ModeInfoBlock
};
struct CRTCInfoBlock{
    uint16_t HorizontalTotal;//Horizontal total in pixels
    uint16_t HorizontalSyncStart;//Horizontal sync start in pixels
    uint16_t HorizontalSyncEnd;//Horizontal sync end in pixels
    uint16_t VerticalTotal;//Vertical total in lines
    uint16_t VerticalSyncStart;//Vertical sync start in lines
    uint16_t VerticalSyncEnd;//Vertical sync end in lines
    uint8_t  Flags;//Flags (Interlaced, Double Scan etc)
    uint32_t PixelClock;//Pixel clock in units of Hz
    uint16_t RefreshRate;// rate in units of 0.01 Hz
    uint8_t  Reserved[40];
};
#pragma pack ()