#include "stivale2.h"
#include <stdint.h>
#include <stddef.h>
#include "psf.h"

#pragma once

class Renderer{
public:
    Renderer(stivale2_struct_tag_framebuffer* Sti_framebuffer, uint64_t* Font);

    stivale2_struct_tag_framebuffer* sti_framebuffer;

    uint64_t* BaseAddr;
    uint16_t Height;
    uint16_t Width;
    uint16_t Bpp;
    uint16_t Pitch;
    uint64_t Size;

    uint64_t bgcolor;
    uint64_t color;
    uint64_t* psfbegin;
    PSF_font psf;

    void Init();
    void Clear();
    void SetBgColor(uint64_t color);
    void PlotPixel(int x, int y, uint32_t pixel);
    void putc(unsigned char chr, uint16_t xOff, uint16_t yOff);
};

extern Renderer* kRenderer;