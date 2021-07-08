#include <stddef.h>
#include <stdint.h>

#pragma once

//Framebuffer Structure
typedef struct {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
	unsigned int ModeCount;
	uint32_t NativeMode;
}Framebuffer;