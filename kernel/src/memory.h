#pragma once
#include <stdint.h>
#include "EfiMem.h"

uint64_t GetMemSize(EFI_MEMORY_DESCRIPTOR* MemMap, uint64_t MemMapEntries, uint64_t MapDescSize);
void memset(void* start, uint8_t val, uint64_t num);