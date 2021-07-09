#include "interrupts.h"
#include "../panic.h"
#include "../IO.h"
#include "../cstr.h"
#include "../perhiperals/keyboard.h"

__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame){
    Panic("Page Fault Detected");
    while(true);
}

__attribute__((interrupt)) void DoubleFault_handler(struct interrupt_frame* frame){
    Panic("Double Fault Detected");
}

__attribute__((interrupt)) void GPFault_handler(struct interrupt_frame* frame){
    Panic("General Protection Fault Detected");
}

__attribute__((interrupt)) void OverflowHandler(struct interrupt_frame* frame){
    Panic("Overflow Detected");
}

__attribute__((interrupt)) void StackSegFaultHandler(struct interrupt_frame* frame){
    Panic("Stack Segment Fault Detected");
}

__attribute__((interrupt)) void KeyboardInterrupt_handler(struct interrupt_frame* frame){
    uint8_t scancode = inb(0x60); //Get the key scancode
    HandleKeyboard(scancode); //Handle keyboard scancode
    PIC_EndMaster(); //Clear the interrupt
}

void RemapPIC(){
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
    io_wait();
}

void PIC_EndMaster(){
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave(){
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}