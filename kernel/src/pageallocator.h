#pragma once
#include "bitmap.h"
#include "EfiMem.h"
#include <stdint.h>
#include "memory.h"
#include <stddef.h>

class PageAllocator{
    public:

    Bitmap PageBitmap;
    
    void ReadEfiMemMap(EFI_MEMORY_DESCRIPTOR* MemMap, size_t MemMapSize, size_t MemMapDescSize);

    void freepage(void* addr);

    void lockpage(void* addr);
    void lockpages(void* addr, uint64_t pageCount);
    
    void* requestpage();

    uint64_t GetFreeRAM();
    uint64_t GetUsedRAM();
    uint64_t GetReservedRAM();

    void initbitmap(size_t Bitmapsize, void* bufferaddr);
   

    private:
    
    void reservepage(void* addr);
    void unreservepage(void* addr);
    
    void reservePages(void* addr, uint64_t pageCount);
    void unreservePages(void* addr, uint64_t pageCount);
};