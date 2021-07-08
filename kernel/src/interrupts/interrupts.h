#pragma once
#include "/home/gktdev/Kronos/commonlib/framebuffer.h"
#include "../BasicRenderer.h"

struct interrupt_frame;
__attribute__((interrupt)) void PageFault_handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void DoubleFault_handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void GPFault_handler(struct interrupt_frame* frame);