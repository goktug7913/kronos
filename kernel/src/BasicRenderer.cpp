#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font){
    TargetFramebuffer = targetFramebuffer;
    PSF1_Font = psf1_Font;
    Colour = 0xffffffff;
    CursorPosition = {0, 0};
}

void BasicRenderer::Print(const char* str){
    
    char* chr = (char*)str;
    while(*chr != 0){
        PutChar(*chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;
        if(CursorPosition.X + 8 > TargetFramebuffer->Width){Next();}
        chr++;
    }
}


void BasicRenderer::PutChar(char chr, unsigned int xOff, unsigned int yOff){
    unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    char* fontPtr = (char*)PSF1_Font->glyphBuf + (chr * PSF1_Font->psf1_header->charsize);
    for (unsigned long y = yOff; y < yOff + 18; y++){
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

void BasicRenderer::Clear(uint32_t ClearColour){
    uint64_t fbBase = (uint64_t)TargetFramebuffer->BaseAddress;
    uint64_t bytesPerScanline = TargetFramebuffer->PixelsPerScanLine * 4; //Each pixel is 4 bytes
    uint64_t fbHeight = TargetFramebuffer->Height;
    uint64_t fbSize = TargetFramebuffer->BufferSize;

    for (int vScanline= 0; vScanline < fbHeight; vScanline++){
        uint64_t pixPtrBase = fbBase + (bytesPerScanline * vScanline);
        for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr++){
            *pixPtr = ClearColour;
        }
    }
}

void BasicRenderer::ClearChar(){

    if (CursorPosition.X == 0){
        CursorPosition.X = CursorPosition.Y = 0;
        CursorPosition.Y -= 18;
        if (CursorPosition.Y < 0) CursorPosition.Y = 0;
    }

    unsigned int xOff = CursorPosition.X;
    unsigned int yOff = CursorPosition.Y;

    unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + 18; y++){
        for (unsigned long x = xOff - 8; x < xOff; x++){
                    *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanLine)) = ClearColour;
        }
    }

    CursorPosition.X -= 8;

    if (CursorPosition.X < 0){
        CursorPosition.X = TargetFramebuffer->Width;
        CursorPosition.Y -= 18;
        if (CursorPosition.Y < 0) CursorPosition.Y = 0;
    }

}

void BasicRenderer::checkLastLine(){
    if (CursorPosition.Y + 18 + 18 >= (long)GlobalRenderer->TargetFramebuffer->Height){
        Scroll(1,1);
        return;
    }
CursorPosition.Y += 18;
}

void BasicRenderer::Next(){
    CursorPosition.X = 0;
    checkLastLine(); // Avoid escaping framebuffer
}

void BasicRenderer::Scroll(uint8_t lines, bool up_dwn){

}