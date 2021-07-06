#include "memory.h"

uint64_t GetMemSize(EFI_MEMORY_DESCRIPTOR* MemMap, uint64_t MemMapEntries, uint64_t MapDescSize){

    static uint64_t totalmembytes = 0;

    if ( totalmembytes > 0) return totalmembytes;

    for(int i = 0; i < MemMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemMap + (i * MapDescSize));
        totalmembytes += desc->numPages * 4096;
    }
    
    return totalmembytes;
}