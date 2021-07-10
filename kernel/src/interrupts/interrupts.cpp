#include "interrupts.h"
#include "../panic.h"
#include "../IO.h"
#include "../cstr.h"
#include "../perhiperals/keyboard.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void DivideByZero_handler(struct interrupt_frame* frame){
    Panic("Exception: Divide by Zero Detected");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void Debug_handler(struct interrupt_frame* frame){
    GlobalRenderer->Print("----Debug Interrupt----");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void NMI_handler(struct interrupt_frame* frame){
    Panic("Exception: Non-Maskable Interrupt!!!");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void Breakpoint_handler(struct interrupt_frame* frame){
    Panic("----Breakpoint Interrupt----");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void OverflowHandler(struct interrupt_frame* frame){
    Panic("Exception: Overflow Detected");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void BoundRangeExceeded_handler(struct interrupt_frame* frame){
    Panic("Exception: Bound Range Exceeded");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void InvalidOpcode_handler(struct interrupt_frame* frame){
    Panic("Exception: Invalid Opcode");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void DeviceNotFound_handler(struct interrupt_frame* frame){
    Panic("Exception: Device Not Found FPU/MMX/SSE");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void DoubleFault_handler(struct interrupt_frame* frame){
    Panic("Exception: Double Fault Detected");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void InvalidTSS_handler(struct interrupt_frame* frame){
    Panic("Exception: Invalid TSS");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void SegNotPresent_handler(struct interrupt_frame* frame){
    Panic("Exception: Segment Not Present!");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void StackSegFaultHandler(struct interrupt_frame* frame){
    Panic("Exception: Stack Segment Fault Detected");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void GPFault_handler(struct interrupt_frame* frame){
    Panic("Exception: General Protection Fault Detected");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame){
    Panic("Exception: Page Fault Detected");
    while(true);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
__attribute__((interrupt)) void KeyboardInterrupt_handler(struct interrupt_frame* frame){
    uint8_t scancode = inb(0x60);   //Get the key scancode
    HandleKeyboard(scancode);       //Handle keyboard scancode
    PIC_EndMaster();                //Clear the interrupt
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
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