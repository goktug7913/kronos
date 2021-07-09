#pragma once
#include <stdint.h>

namespace QWERTY{
    
    #define Released    0x80

    #define LShift      0x2A
    #define RShift      0x36

    #define Enter       0x1C
    #define Backspace   0x0E
    #define Space       0x39

    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}