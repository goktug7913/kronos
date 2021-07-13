#include "renderer.h"

Renderer::Renderer(stivale2_struct_tag_framebuffer* sti_framebuffer){
        
        int16_t cX = 0;
        int16_t cY = 0;

        uint64_t* BaseAddr = (uint64_t*)sti_framebuffer->framebuffer_addr;
        uint16_t Height = sti_framebuffer->framebuffer_height;
        uint16_t Width = sti_framebuffer->framebuffer_width;
        uint16_t Bpp = sti_framebuffer->framebuffer_bpp;
        uint16_t Pitch = sti_framebuffer->framebuffer_pitch;
        uint64_t Size = Pitch*Height;
    }