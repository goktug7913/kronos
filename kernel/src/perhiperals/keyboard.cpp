#include "/home/gktdev/Kronos/commonlib/framebuffer.h"
#include "keyboard.h"

bool bLShift;
bool bRShift;

void HandleKeyboard(uint8_t scancode){
    
    switch (scancode){

    case LShift:
        bLShift = true;
        return;

    case LShift + Released:
        bLShift = false;
        break;
    
    case RShift:
            bLShift = true;
        return;

    case RShift + Released:
        bLShift = false;
        return;

    case Enter:
        GlobalRenderer->Next();
        return;

    case Backspace:
        GlobalRenderer->ClearChar();
        return;

    case Space:
        GlobalRenderer->PutChar(' ');
        return;
    }
    
    char ASCII = QWERTY::Translate(scancode, bLShift | bRShift);
    
    if(ASCII != 0){
        GlobalRenderer->PutChar(ASCII);
    }
}