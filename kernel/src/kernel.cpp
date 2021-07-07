#include <stdint.h>

#include "EfiMem.h" //Needs to be included first, fix dependency issue later
#include "bootnfo.h" //Add EfiMem header
#include "memory.h"
#include "PSF.h"
#include "framebuffer.h"
#include "KronosRenderer.h"
#include "cstr.h"
#include "bitmap.h"
#include "paging/pageallocator.h"
#include "paging/pagemapindexer.h"
#include "paging/paging.h"
#include "paging/pagetablemgr.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

PageAllocator GlobalAllocator;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

//KERNEL ENTRY

extern "C" void _start(bootnfo* bootinfo){
	
	KronosRenderer Renderer(bootinfo->framebuffer, bootinfo->psf1_font);
	Renderer.Cursor = {0, 0};

	GlobalAllocator = PageAllocator();
	GlobalAllocator.ReadEfiMemMap(bootinfo->MemMap, bootinfo->MapSize, bootinfo->DescSize);

	Renderer.Cursor = {0, Renderer.Cursor.y+16};

	//Find UEFI Memory Map Entry Count
	uint64_t MemMapEntries = bootinfo->MapSize / bootinfo->DescSize;

	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

	GlobalAllocator.lockpages(&_KernelStart, kernelPages);
	Renderer.print(0xfffffff, "Kernel memory locked.");
	
		//Print memory stats
		Renderer.print(0xfffffff, "Total RAM: ");
		Renderer.print(0xff00fff, tostr(GetMemSize(bootinfo->MemMap, MemMapEntries, bootinfo->DescSize) / 1024)); // Convert to KB
		Renderer.print(0xff00fff, " Kbytes");
		Renderer.Cursor = {0, Renderer.Cursor.y+16};
		
		Renderer.print(0xfffffff, "Free RAM: ");
		Renderer.print(0xff00fff, tostr(GlobalAllocator.GetFreeRAM() / 1024)); // Convert to KB
		Renderer.print(0xff00fff, " Kbytes");
		Renderer.Cursor = {0, Renderer.Cursor.y+16};

		Renderer.print(0xfffffff, "Used RAM: ");
		Renderer.print(0xff00fff, tostr(GlobalAllocator.GetUsedRAM() / 1024)); // Convert to KB
		Renderer.print(0xff00fff, " Kbytes");
		Renderer.Cursor = {0, Renderer.Cursor.y+16};

		Renderer.print(0xfffffff, "Reserved RAM: ");
		Renderer.print(0xff00fff, tostr(GlobalAllocator.GetReservedRAM() / 1024)); // Convert to KB
		Renderer.print(0xff00fff, " Kbytes");
		Renderer.Cursor = {0, Renderer.Cursor.y+16};

	for (uint64_t i = 0; i < 5; i++){
		void* pageadr = GlobalAllocator.requestpage();
		Renderer.print(0xfffffff, "Page "); Renderer.print(0xfffffff, tostr(i)); Renderer.print(0xfffffff, ": ");
		Renderer.print(0x00fffffff, h_tostr((uint64_t)pageadr));
		Renderer.Cursor = {0, Renderer.Cursor.y+16};
	}


	PageTable* PML4 = (PageTable*)GlobalAllocator.requestpage();
    memset(PML4, 0, 0x1000);

    PageTableManager pageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemSize(bootinfo->MemMap, MemMapEntries, bootinfo->DescSize); t+= 0x1000){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootinfo->framebuffer->baseAddr;
    uint64_t fbSize = (uint64_t)bootinfo->framebuffer->bufSize + 0x1000;

    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

    pageTableManager.MapMemory((void*)0x600000000, (void*)0x80000);

    uint64_t* test = (uint64_t*)0x600000000;
    *test = 26;

    Renderer.print(0xfffffff, tostr(*test));

	return;
};