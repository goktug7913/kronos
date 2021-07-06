#pragma once
#include "framebuffer.h"
#include "PSF.h"
#include "math.h"

class KronosRenderer{

public:

	KronosRenderer(st_Framebuffer* framebuffer, PSF1_FONT* font);
	
	st_Framebuffer* r_framebuffer;
	PSF1_FONT* r_font;
	intPoint Cursor;

	void putc(unsigned int color, char chr, unsigned int xOffset, unsigned int yOffset);
	void print(unsigned int color, const char* str);

private:

};