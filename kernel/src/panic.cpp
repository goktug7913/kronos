#include "panic.h"
#include "BasicRenderer.h"

void Panic(const char* message){
    GlobalRenderer->Clear(0x7a0000);

    GlobalRenderer->CursorPosition = {0, 0};

    GlobalRenderer->ClearColour = 0x520000;

    GlobalRenderer->Print("-Kernel Panic-");
    GlobalRenderer->Next();
    GlobalRenderer->Next();
    GlobalRenderer->Print(message);
}