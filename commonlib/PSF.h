#include <stddef.h>

#pragma once

#define PSF1_MAGIC0 0X36
#define PSF1_MAGIC1 0X04

typedef struct
{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;

} PSF1_HEADER;

typedef struct
{
	PSF1_HEADER* psf1_header;
	void* glyphBuf;

} PSF1_FONT;

