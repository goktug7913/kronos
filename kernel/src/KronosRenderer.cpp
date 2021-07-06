#include "KronosRenderer.h"
#include "framebuffer.h"
#include "PSF.h"
#include "math.h"

KronosRenderer::KronosRenderer(st_Framebuffer* framebuffer, PSF1_FONT* font){
    r_framebuffer = framebuffer;
    r_font = font;
    Cursor = {0, 0};
}

void KronosRenderer::putc(unsigned int color, char chr, unsigned int xOffset, unsigned int yOffset){

	unsigned int* pxPtr = (unsigned int*)r_framebuffer->baseAddr;
	char* fontPtr = (char*)r_font->glyphBuf + (chr * r_font->psf1_header->charsize);

	for (unsigned long y = yOffset; y < yOffset + 18 ; y++){

		for(unsigned long x = xOffset; x < xOffset + 8 ; x++){

			if((*fontPtr &(0b10000000 >> (x - xOffset))) > 0){

				*(unsigned int*)(pxPtr + x + (y * r_framebuffer->PixPerScanline)) = color;

			}
		}
		fontPtr++;
	}
	
}

void KronosRenderer::print(unsigned int color, const char* str){

	char* chr = (char*)str;
	
	while (*chr != 0){
		putc(color, *chr, Cursor.x, Cursor.y);
		Cursor.x += 8;

		if(Cursor.x + 8 > r_framebuffer->Width){
			Cursor.x = 0;
			Cursor.y += 16;
		}
	
		chr++;
	}
}