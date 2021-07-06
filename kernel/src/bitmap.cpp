#include "bitmap.h"

bool Bitmap::operator[](uint64_t index){
    uint64_t byteindex = index / 8;
    uint8_t bitindex = index % 8;
    uint8_t bitindexer = 0b10000000 >> bitindex;
    if((buffer[byteindex] & bitindexer) > 0){
        return true;
    }
    return false;
}

void Bitmap::set(uint64_t index, bool value){
    uint64_t byteindex = index / 8;
    uint8_t bitindex = index % 8;
    uint8_t bitindexer = 0b10000000 >> bitindex;
    buffer[byteindex] &= ~bitindexer;
    if (value){
        buffer[byteindex] |= bitindexer;
    }
}