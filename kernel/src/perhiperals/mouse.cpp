#include "mouse.h"
#include "../IO.h"

void MouseWait(){
    uint64_t timeout = 100000;
    while (timeout--){if((inb(0x64) & 0b10) == 0){return;}}
};

void MouseWaitInput(){
    uint64_t timeout = 100000;
    while (timeout--){if(inb(0x64) & 0b1){return;}}
};

void MouseWrite(uint8_t val){
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x64, val);

}

uint8_t MouseRead(){
    MouseWaitInput();
    return inb(0x60);
}

void PS2MouseInit(){
    outb(0x64, 0xA8); //Enable AUX device mouse
    
    MouseWait();
    outb(0x64, 0x20); //Request to send data from the controller
    MouseWaitInput();

    uint8_t status = inb(0x60);
    status |= 0b10;
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status); // Set the correct bit "compaq"
    
    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
};