#include <stddef.h>
#include <stdint.h>

#pragma once

//Framebuffer Structure
typedef struct {
	void* baseAddr;
	size_t bufSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixPerScanline;
	unsigned int ModeCount;
	uint32_t NativeMode;
	
}st_Framebuffer;