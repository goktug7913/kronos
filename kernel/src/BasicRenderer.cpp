#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font)
{
    TargetFramebuffer = targetFramebuffer;
    PSF1_Font = psf1_Font;
    Colour = 0xffffffff;
    CursorPosition = {0, 0};
}

void BasicRenderer::Print(const char* str)
{
    
    char* chr = (char*)str;
    while(*chr != 0){
        PutChar(*chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;
        if(CursorPosition.X + 8 > TargetFramebuffer->Width)
        {
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }
        chr++;
    }
}

void BasicRenderer::PutChar(char chr, unsigned int xOff, unsigned int yOff){
    unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    char* fontPtr = (char*)PSF1_Font->glyphBuf + (chr * PSF1_Font->psf1_header->charsize);
    for (unsigned long y = yOff; y < yOff + 16; y++){
        for (unsigned long x = xOff; x < xOff+8; x++){
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0){
                    *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanLine)) = Colour;
                }

        }
        fontPtr++;
    }
}

void BasicRenderer::PutChar(char chr){
    PutChar(chr, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X += 8; //Next Character
    if (CursorPosition.X + 8 > TargetFramebuffer->Width){BasicRenderer::Next();} //If at the edge of screen, nextline
}

void BasicRenderer::Clear(uint32_t colour){
    uint64_t fbBase = (uint64_t)TargetFramebuffer->BaseAddress;
    uint64_t bytesPerScanline = TargetFramebuffer->PixelsPerScanLine * 4; //Each pixel is 4 bytes
    uint64_t fbHeight = TargetFramebuffer->Height;
    uint64_t fbSize = TargetFramebuffer->BufferSize;

    for (int vScanline; vScanline < fbHeight; vScanline++){
        uint64_t pixPtrBase = fbBase + (bytesPerScanline * vScanline);
        for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)pixPtrBase + bytesPerScanline; pixPtr++){
            *pixPtr = ClearColour;
        }
    }
}

void BasicRenderer::ClearChar(){

    if (CursorPosition.X == 0){                           // If we are at the beginning of a new line
        CursorPosition.X = TargetFramebuffer->Width;      // Go to end of the line
        CursorPosition.Y -= 16;                           // Go up a line
        if(CursorPosition.Y < 0){CursorPosition.Y = 0;}   // Out of bounds check
    }
    
    unsigned int xOff = CursorPosition.X;
    unsigned int yOff = CursorPosition.Y;

        unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + 16; y++){
        
        for (unsigned long x = xOff - 8; x < xOff; x++){
            
            *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanLine)) = ClearColour;
        }
    
        if (CursorPosition.X == 0){                           // If we are at the beginning of a new line
            CursorPosition.X = TargetFramebuffer->Width;      // Go to end of the line
            CursorPosition.Y -= 16;                           // Go up a line
            if(CursorPosition.Y < 0){CursorPosition.Y = 0;}   // Out of bounds check
        }
    }
}

void BasicRenderer::Next(){
    CursorPosition.X = 0;
    CursorPosition.Y += 16;
}