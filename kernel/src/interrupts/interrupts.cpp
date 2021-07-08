#include "interrupts.h"

__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame){
    GlobalRenderer->Print("Page Fault Interrupt");
    
    while(true);
}