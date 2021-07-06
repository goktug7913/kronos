#pragma once

#include "PSF.h"
#include "framebuffer.h"

typedef struct 
{
    st_Framebuffer* framebuffer;
    PSF1_FONT* psf1_font;
    EFI_MEMORY_DESCRIPTOR* MemMap;
	uint64_t MapSize;
	uint64_t DescSize;
} bootnfo;
