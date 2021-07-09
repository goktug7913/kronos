#pragma once
#include "/home/gktdev/Kronos/commonlib/framebuffer.h"
#include "../BasicRenderer.h"

//PIC remap helpers
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01
//- - - - - - - - - - - 

struct interrupt_frame;
__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void DoubleFault_handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void GPFault_handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void OverflowHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void KeyboardInterrupt_handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void StackSegFaultHandler(struct interrupt_frame* frame);


void RemapPIC();

void PIC_EndMaster(); //Clear interrupt from master PIC
void PIC_EndSlave();  //Clear interrupt from slave PIC