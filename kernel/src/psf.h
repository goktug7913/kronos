#include <stddef.h>
#pragma once

#define PSF_FONT_MAGIC 0x864ab572
 
typedef struct {
    uint32_t magic = PSF_FONT_MAGIC;    /* magic bytes to identify PSF */
    uint32_t version = 0;               /* zero */
    uint32_t headersize = 32;                /* offset of bitmaps in file, 32 */
    uint32_t flags = 0;                     /* 0 if there's no unicode table */
    uint32_t numglyph;                  /* number of glyphs */
    uint32_t bytesperglyph;             /* size of each glyph */
    uint32_t height = 16;               /* height in pixels */
    uint32_t width = 8;                 /* width in pixels */
} PSF_font;