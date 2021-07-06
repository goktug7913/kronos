#pragma once
#include <stdint.h>
#include "EfiMem.h"

uint64_t GetMemSize(EFI_MEMORY_DESCRIPTOR* MemMap, uint64_t MemMapEntries, uint64_t MapDescSize);