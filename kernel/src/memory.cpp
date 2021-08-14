#include "memory.h"

void memset(void* start, uint8_t value, uint64_t end){
    for (uint64_t offset = 0; offset < end; offset++){
        *(uint8_t*)((uint64_t)start + offset) = value;
    }
}