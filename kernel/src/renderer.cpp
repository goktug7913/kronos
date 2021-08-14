#include "renderer.h"

Renderer* kRenderer;

Renderer::Renderer(stivale2_struct_tag_framebuffer* Sti_framebuffer, uint64_t* Font){
    sti_framebuffer = Sti_framebuffer;
    uint64_t* psfbegin = Font;
    int16_t cX = 0;
    int16_t cY = 0;
    uint64_t bgcolor;

    PSF_font psf = *(PSF_font*)psfbegin;
}

void Renderer::Init(){
    BaseAddr = (uint64_t*)sti_framebuffer->framebuffer_addr;
    Height = sti_framebuffer->framebuffer_height;
    Width = sti_framebuffer->framebuffer_width;
    Pitch = sti_framebuffer->framebuffer_pitch;
    Bpp = sti_framebuffer->framebuffer_bpp;
    Size = Pitch*Height;
}

void Renderer::SetBgColor(uint64_t color){bgcolor = color;}

void Renderer::Clear(){
    for(uint64_t t = 0; t < Height; t++){
        for(uint64_t i = 0; i < Width; i++){
            PlotPixel(i, t, bgcolor);
            //*(uint32_t*)((uint64_t)BaseAddr + (i*4) + (t * Pitch)) = bgcolor;
        }
    }
}

void Renderer::PlotPixel(int x, int y, uint32_t pixel){
    *(uint32_t*)((uint64_t)BaseAddr + (x*4) + (y * Pitch)) = pixel;
}

void Renderer::putc(unsigned char chr, uint16_t xOff, uint16_t yOff){
    char* fontPtr = ((char*)(psfbegin + psf.headersize + (chr * psf.bytesperglyph)));
    
    for (uint16_t  y = yOff; y < yOff + 16; y++){
        for (uint16_t  x = xOff; x < xOff+8; x++){
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0){
                PlotPixel(x, y, color);
            }
        }
        fontPtr++;
    }
}