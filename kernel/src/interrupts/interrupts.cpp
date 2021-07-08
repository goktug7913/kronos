#include "interrupts.h"
#include "../panic.h"

__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame){
    Panic("Page Fault Detected");

    int* test = (int*)0x80000000000;

    *test = 2;
    while(true);
}

__attribute__((interrupt)) void DoubleFault_handler(struct interrupt_frame* frame){
    Panic("Double Fault Detected");
}

__attribute__((interrupt)) void GPFault_handler(struct interrupt_frame* frame){
    Panic("General Protection Fault Detected");
}