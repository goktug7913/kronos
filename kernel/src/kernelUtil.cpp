#include "kernelUtil.h"
#include "gdt/gdt.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "IO.h"
#include "stivale2.h"

KernelInfo kernelInfo; 
PageTableManager pageTableManager = NULL;

// Helper function which will allow us to scan for tags
// that we want FROM the bootloader (structure tags).
extern "C" void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {
        // If the tag pointer is NULL (end of linked list), we did not find
        // the tag. Return NULL to signal this.
        if (current_tag == NULL) {
            return NULL;
        }
 
        // Check whether the identifier matches. If it does, return a pointer
        // to the matching tag.
        if (current_tag->identifier == id) {
            return current_tag;
        }
 
        // Get a pointer to the next tag in the linked list and repeat.
        current_tag = (void *)current_tag->next;
    }
}

void PrepareMemory(BootInfo* bootInfo){
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    pageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t+= 0x1000){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
    GlobalAllocator.LockPages((void*)fbBase, fbSize/ 0x1000 + 1);
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

    kernelInfo.pageTableManager = &pageTableManager;
}

IDTR idtr;

//----------------------------------------------------------------------------------------------------------------------------------------
/*void SetIDTGate(void* handler, uint8_t entryoffset, uint8_t type_attr, uint8_t selector){} IMPLEMENT THIS*/
//----------------------------------------------------------------------------------------------------------------------------------------

void PrepareInterrupts(){
    
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();
    
    { // Fill The IDT with interrupt handlers

        IDTDescEntry* int_DivideByZero = (IDTDescEntry*)(idtr.Offset + 0x0 * sizeof(IDTDescEntry));
        int_DivideByZero->SetOffset((uint64_t)DivideByZero_handler);
        int_DivideByZero->type_attr = IDT_TA_InterruptGate;
        int_DivideByZero->selector = 0x08;

        IDTDescEntry* int_Debug = (IDTDescEntry*)(idtr.Offset + 0x1 * sizeof(IDTDescEntry));
        int_Debug->SetOffset((uint64_t)Debug_handler);
        int_Debug->type_attr = IDT_TA_InterruptGate;
        int_Debug->selector = 0x08;

        IDTDescEntry* int_NMI = (IDTDescEntry*)(idtr.Offset + 0x2 * sizeof(IDTDescEntry));
        int_NMI->SetOffset((uint64_t)NMI_handler);
        int_NMI->type_attr = IDT_TA_InterruptGate;
        int_NMI->selector = 0x08;

        IDTDescEntry* int_Breakpoint = (IDTDescEntry*)(idtr.Offset + 0x3 * sizeof(IDTDescEntry));
        int_Breakpoint->SetOffset((uint64_t)Breakpoint_handler);
        int_Breakpoint->type_attr = IDT_TA_TrapGate;
        int_Breakpoint->selector = 0x08;

        IDTDescEntry* int_Overflow = (IDTDescEntry*)(idtr.Offset + 0x4 * sizeof(IDTDescEntry));
        int_Overflow->SetOffset((uint64_t)OverflowHandler);
        int_Overflow->type_attr = IDT_TA_TrapGate;
        int_Overflow->selector = 0x08;

        IDTDescEntry* int_BoundRangeExceeded = (IDTDescEntry*)(idtr.Offset + 0x5 * sizeof(IDTDescEntry));
        int_Breakpoint->SetOffset((uint64_t)BoundRangeExceeded_handler);
        int_Breakpoint->type_attr = IDT_TA_InterruptGate;
        int_Breakpoint->selector = 0x08;

        IDTDescEntry* int_InvalidOpcode = (IDTDescEntry*)(idtr.Offset + 0x6 * sizeof(IDTDescEntry));
        int_InvalidOpcode->SetOffset((uint64_t)InvalidOpcode_handler);
        int_InvalidOpcode->type_attr = IDT_TA_InterruptGate;
        int_InvalidOpcode->selector = 0x08;

        IDTDescEntry* int_DeviceNotFound = (IDTDescEntry*)(idtr.Offset + 0x7 * sizeof(IDTDescEntry));
        int_DeviceNotFound->SetOffset((uint64_t)DeviceNotFound_handler);
        int_DeviceNotFound->type_attr = IDT_TA_InterruptGate;
        int_DeviceNotFound->selector = 0x08;

        IDTDescEntry* int_DoubleFault = (IDTDescEntry*)(idtr.Offset + 0x8 * sizeof(IDTDescEntry));
        int_DoubleFault->SetOffset((uint64_t)DoubleFault_handler);
        int_DoubleFault->type_attr = IDT_TA_InterruptGate;
        int_DoubleFault->selector = 0x08;

        IDTDescEntry* int_InvalidTSS = (IDTDescEntry*)(idtr.Offset + 0xA * sizeof(IDTDescEntry));
        int_InvalidTSS->SetOffset((uint64_t)InvalidTSS_handler);
        int_InvalidTSS->type_attr = IDT_TA_InterruptGate;
        int_InvalidTSS->selector = 0x08;

        IDTDescEntry* int_SegNotPresent = (IDTDescEntry*)(idtr.Offset + 0xB * sizeof(IDTDescEntry));
        int_SegNotPresent->SetOffset((uint64_t)SegNotPresent_handler);
        int_SegNotPresent->type_attr = IDT_TA_InterruptGate;
        int_SegNotPresent->selector = 0x08;

        IDTDescEntry* int_StackSegmentFault = (IDTDescEntry*)(idtr.Offset + 0xC * sizeof(IDTDescEntry));
        int_StackSegmentFault->SetOffset((uint64_t)StackSegFaultHandler);
        int_StackSegmentFault->type_attr = IDT_TA_InterruptGate;
        int_StackSegmentFault->selector = 0x08;

        IDTDescEntry* int_GPFault = (IDTDescEntry*)(idtr.Offset + 0xD * sizeof(IDTDescEntry));
        int_GPFault->SetOffset((uint64_t)GPFault_handler);
        int_GPFault->type_attr = IDT_TA_InterruptGate;
        int_GPFault->selector = 0x08;
        
        IDTDescEntry* int_PageFault = (IDTDescEntry*)(idtr.Offset + 0xE * sizeof(IDTDescEntry));
        int_PageFault->SetOffset((uint64_t)PageFault_handler);
        int_PageFault->type_attr = IDT_TA_InterruptGate;
        int_PageFault->selector = 0x08;

        IDTDescEntry* int_Keyboard = (IDTDescEntry*)(idtr.Offset + 0x21 * sizeof(IDTDescEntry));
        int_Keyboard->SetOffset((uint64_t)KeyboardInterrupt_handler);
        int_Keyboard->type_attr = IDT_TA_InterruptGate;
        int_Keyboard->selector = 0x08;

        IDTDescEntry* int_Mouse = (IDTDescEntry*)(idtr.Offset + 0x2C * sizeof(IDTDescEntry));
        int_Mouse->SetOffset((uint64_t)MouseInterrupt_handler);
        int_Mouse->type_attr = IDT_TA_InterruptGate;
        int_Mouse->selector = 0x08;
    }

    asm ("lidt %0" : : "m" (idtr));
    
    RemapPIC();

    asm ("sti"); //Enable Maskable Interrupts
}
stivale2_module stub;
BasicRenderer r = BasicRenderer((stivale2_struct_tag_framebuffer*)NULL, stub); //takes stivale2_struct_tag_framebuffer AND stivale2_module

KernelInfo InitializeKernel(struct stivale2_struct *bootInfo){
    
    auto *modules = (stivale2_struct_tag_modules *) stivale2_get_tag(bootInfo,STIVALE2_STRUCT_TAG_MODULES_ID);
    auto *memmap = (stivale2_struct_tag_memmap *) stivale2_get_tag(bootInfo, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    auto *sti_framebuffer = (stivale2_struct_tag_framebuffer*) stivale2_get_tag(bootInfo, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

    stivale2_module font = *modules->modules;

    r = BasicRenderer(sti_framebuffer, font);
    GlobalRenderer = &r;

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    //PrepareMemory(bootInfo);

    memset((void*)sti_framebuffer->framebuffer_addr, 0, sti_framebuffer->framebuffer_pitch*sti_framebuffer->framebuffer_height);

    PrepareInterrupts();

    InitPS2Mouse();

    //Unmask PIC chips
    outb(PIC1_DATA, 0b11111001);
    outb(PIC2_DATA, 0b11111111);

    return kernelInfo;
}