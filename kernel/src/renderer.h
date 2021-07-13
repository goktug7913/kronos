#pragma once
#include "stivale2.h"
#include <stdint.h>
#include <stddef.h>

class Renderer{
public:
    Renderer(stivale2_struct_tag_framebuffer* sti_framebuffer);
    
    uint64_t* BaseAddr;
    uint16_t Height;
    uint16_t Width;
    uint16_t Bpp;
    uint16_t Pitch;
    uint64_t Size;
};

extern Renderer* kRenderer;