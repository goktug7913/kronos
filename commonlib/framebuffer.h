#include <stddef.h>
#include <stdint.h>

#pragma once

//Framebuffer Structure
typedef struct {
	uint64_t* BaseAddress;
	size_t BufferSize;
	uint16_t Width;
	uint16_t Height;
	unsigned int PixelsPerScanLine;
	uint16_t pitch;
    uint16_t bpp;

    uint8_t  memory_model;

    uint8_t  red_mask_size;
    uint8_t  red_mask_shift;

    uint8_t  green_mask_size;
    uint8_t  green_mask_shift;
	
    uint8_t  blue_mask_size;
    uint8_t  blue_mask_shift;

	unsigned int ModeCount;
	uint32_t NativeMode;
}Framebuffer;