#include "panic.h"
#include "BasicRenderer.h"

void Panic(const char* message){
    GlobalRenderer->Clear(0x00ff0011);

    GlobalRenderer->CursorPosition = {0, 0};

    GlobalRenderer->Colour = 0;

    GlobalRenderer->Print("-Kernel Panic-");
    GlobalRenderer->Next();
    GlobalRenderer->Next();
    GlobalRenderer->Print(message);
}