#include "type.h"

#ifndef _VIDEO_H_
#define _VIDEO_H_

struct VideoInfo {
    uint64 addr;
    uint32 width;
    uint32 height;
    uint8  bits;
    uint32 pitch;
    uint8  type;
};

void init_video(void* boot_info_addr,struct VideoInfo *videoInfo);

#endif