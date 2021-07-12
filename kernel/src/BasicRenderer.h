#pragma once
#include "math.h"
#include "framebuffer.h"
#include "PSF.h"
#include "paging/PageFrameAllocator.h"
#include "stivale2.h"

class BasicRenderer{
    public:
    BasicRenderer(stivale2_struct_tag_framebuffer* targetFramebuffer, stivale2_module psf1_Font);
    Point CursorPosition;
    Framebuffer* TargetFramebuffer; //GOP Framebuffer Address
    stivale2_module PSF1_Font;
    unsigned int Colour;
    unsigned int ClearColour = 0x000a1a;

    void Print(const char* str);

    void PutChar(char chr, unsigned int xOff, unsigned int yOff);
    void PutChar(char chr);
    void ClearChar();
    void Clear(uint32_t colour);
    void Next();
    void checkLastLine();
    void Scroll(uint8_t lines, bool up_dwn);
};

extern BasicRenderer* GlobalRenderer;