#pragma once
#include <stdint.h>
#include <stddef.h>

class Bitmap{
    public:
    size_t size;
    uint8_t* buffer;
    bool operator[](uint64_t index);

    void set(uint64_t index, bool val);
};