#pragma once
#include "math.h"
#include "framebuffer.h"
#include "PSF.h"
#include "paging/PageFrameAllocator.h"

class BasicRenderer{
    public:
    BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font);
    Point CursorPosition;
    Framebuffer* TargetFramebuffer; //GOP Framebuffer Address
    PSF1_FONT* PSF1_Font;
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