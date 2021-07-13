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

__attribute__((interrupt)) void DivideByZero_handler(struct interrupt_frame* frame);                        //0x0
__attribute__((interrupt)) void Debug_handler(struct interrupt_frame* frame);                               //0x1
__attribute__((interrupt)) void NMI_handler(struct interrupt_frame* frame);                                 //0x2
__attribute__((interrupt)) void Breakpoint_handler(struct interrupt_frame* frame);                          //0x3
__attribute__((interrupt)) void OverflowHandler(struct interrupt_frame* frame);                             //0x4
__attribute__((interrupt)) void BoundRangeExceeded_handler(struct interrupt_frame* frame);                  //0x5
__attribute__((interrupt)) void InvalidOpcode_handler(struct interrupt_frame* frame);                       //0x6
__attribute__((interrupt)) void DeviceNotFound_handler(struct interrupt_frame* frame);                      //0x7
__attribute__((interrupt)) void DoubleFault_handler(struct interrupt_frame* frame, long long int uword_t);  //0x8
__attribute__((interrupt)) void InvalidTSS_handler(struct interrupt_frame* frame, long long int uword_t);   //0xA
__attribute__((interrupt)) void SegNotPresent_handler(struct interrupt_frame* frame, long long int uword_t);//0xB
__attribute__((interrupt)) void StackSegFaultHandler(struct interrupt_frame* frame, long long int uword_t); //0xC
__attribute__((interrupt)) void GPFault_handler(struct interrupt_frame* frame, long long int uword_t);      //0xD
__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame, long long int uword_t);    //0xE
__attribute__((interrupt)) void KeyboardInterrupt_handler(struct interrupt_frame* frame);                   //0x21
__attribute__((interrupt)) void MouseInterrupt_handler(struct interrupt_frame* frame);                      //0x2C


void RemapPIC();

void PIC_EndMaster(); //Clear interrupt from master PIC
void PIC_EndSlave();  //Clear interrupt from slave PIC