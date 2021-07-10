#include "panic.h"
#include "BasicRenderer.h"

void Panic(const char* message){
    GlobalRenderer->Clear(7995392);

    GlobalRenderer->CursorPosition = {0, 0};

    GlobalRenderer->ClearColour = 7995392;

    GlobalRenderer->Print("-Kernel Panic-");
    GlobalRenderer->Next();
    GlobalRenderer->Next();
    GlobalRenderer->Print(message);
}