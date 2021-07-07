#include "pageallocator.h"

uint64_t FreeMem;
uint64_t ReservedMem;
uint64_t UsedMem;

bool initialized = 0;


void PageAllocator::ReadEfiMemMap(EFI_MEMORY_DESCRIPTOR* MemMap, size_t MemMapSize, size_t MemMapDescSize){

    if (initialized) return;

    uint64_t MemMapEntries = MemMapSize / MemMapDescSize;
    void* LargestMemSeg = NULL;
    size_t LargestMemSegSize = 0;

    for (int i = 0; i < MemMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemMap + (i * MemMapDescSize));
        
        if(desc->type == 7){    //EfiConventionalMemory
            if (desc->numPages * 4096 > LargestMemSegSize){
                LargestMemSeg = desc->phyAddr;
                LargestMemSegSize = desc->numPages * 4096;
            }
        }
    }

    uint64_t MemSize = GetMemSize(MemMap, MemMapEntries, MemMapDescSize);
    FreeMem = MemSize;
    uint64_t Bitmapsize = MemSize / 4096 / 8 + 1;
    
    for (int i = 0; i < MemMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemMap + (i * MemMapDescSize));
        if (desc->type != 7){ // not efiConventionalMemory
            reservePages(desc->phyAddr, desc->numPages);
        }
    }

    initbitmap(Bitmapsize, LargestMemSeg);
    lockpages(&PageBitmap, PageBitmap.size / 4096 + 1);
    initialized = true;
    
}

void PageAllocator::initbitmap(size_t Bitmapsize, void* bufferaddr){

    PageBitmap.size = Bitmapsize;
    PageBitmap.buffer = (uint8_t*)bufferaddr;

    for (int i = 0; i < Bitmapsize; i++){ // ZERO FILL PAGE
        *(uint8_t*)(PageBitmap.buffer + i) = 0;
    }
}

void* PageAllocator::requestpage(){
    
    for (uint64_t index = 0; index < PageBitmap.size * 8; index++){
        if (PageBitmap[index == true]) continue;
            lockpage((void*)(index * 4096));
            return (void*)(index * 4096);
    }
    return NULL; //If theres no memory available return nothing, IMPLEMENT PAGE SWAP 
}

void PageAllocator::freepage(void* addr){

    uint64_t index = (uint64_t)addr / 4096; 
    if (PageBitmap[index] == false) return; // Return if already free

    PageBitmap.set(index, false); // Else free the memory page
    FreeMem += 4096;
    UsedMem -= 4096;
}

void PageAllocator::lockpage(void* addr){

    uint64_t index = (uint64_t)addr / 4096; 
    if (PageBitmap[index] == true) return; 

    PageBitmap.set(index, true); 
    FreeMem -= 4096;
    UsedMem += 4096;
}

void PageAllocator::lockpages(void* addr, uint64_t pageCount){

    for (int t = 0; t < pageCount; t++){
        lockpage((void*)((uint64_t)addr + (t * 4096)));
    }
}

void PageAllocator::reservepage(void* addr){

    uint64_t index = (uint64_t)addr / 4096; 
    if (PageBitmap[index] == true) return; 

    PageBitmap.set(index, true);
    FreeMem -= 4096;
    ReservedMem += 4096;
}

void PageAllocator::unreservepage(void* addr){

    uint64_t index = (uint64_t)addr / 4096; 
    if (PageBitmap[index] == false) return; // Return if already free

    PageBitmap.set(index, false); // Else free the memory page
    FreeMem += 4096;
    ReservedMem -= 4096;
}

void PageAllocator::reservePages(void* addr, uint64_t pageCount){

    uint64_t index = (uint64_t)addr / 4096;
    if (PageBitmap[index] == true) return;
    PageBitmap.set(index, true);
    FreeMem -= 4096;
    ReservedMem += 4096;
}

void PageAllocator::unreservePages(void* addr, uint64_t pageCount){

    for (int i = 0; i < pageCount; i++){
        unreservepage((void*)((uint64_t)addr + (i * 4096)));
    }
}

uint64_t PageAllocator::GetFreeRAM() {return FreeMem;}

uint64_t PageAllocator::GetUsedRAM() {return UsedMem;}

uint64_t PageAllocator::GetReservedRAM() {return ReservedMem;}