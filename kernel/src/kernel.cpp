#include <stddef.h>
#include <stdint.h>

#include "EfiMem.h"
#include "framebuffer.h"
#include "PSF.h"
#include "KronosRenderer.h"
#include "cstr.h"
#include "bootnfo.h"
#include "memory.h"
#include "bitmap.h"
#include "pageallocator.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

//KERNEL ENTRY

extern "C" void _start(bootnfo* bootinfo){
	
	KronosRenderer Renderer(bootinfo->framebuffer, bootinfo->psf1_font);
	Renderer.Cursor = {0, 0};

	PageAllocator PgAllocr;
	PgAllocr.ReadEfiMemMap(bootinfo->MemMap, bootinfo->MapSize, bootinfo->DescSize);

	//uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    //uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

	//Find UEFI Memory Map Entry Count
	uint64_t MemMapEntries = bootinfo->MapSize / bootinfo->DescSize;
	
	
		//Print memory stats
		Renderer.print(0xfffffff, "Total RAM: ");
		Renderer.print(0xff00fff, tostr(GetMemSize(bootinfo->MemMap, MemMapEntries, bootinfo->DescSize) / 1024)); // Convert to KB
		Renderer.print(0xff00fff, " Kbytes");
		Renderer.Cursor = {0, Renderer.Cursor.y+16};
		
		Renderer.print(0xfffffff, "Free RAM: ");
		Renderer.print(0xff00fff, tostr(PgAllocr.GetFreeRAM() / 1024)); // Convert to KB
		Renderer.print(0xff00fff, " Kbytes");
		Renderer.Cursor = {0, Renderer.Cursor.y+16};

		Renderer.print(0xfffffff, "Used RAM: ");
		Renderer.print(0xff00fff, tostr(PgAllocr.GetUsedRAM() / 1024)); // Convert to KB
		Renderer.print(0xff00fff, " Kbytes");
		Renderer.Cursor = {0, Renderer.Cursor.y+16};

		Renderer.print(0xfffffff, "Reserved RAM: ");
		Renderer.print(0xff00fff, tostr(PgAllocr.GetReservedRAM() / 1024)); // Convert to KB
		Renderer.print(0xff00fff, " Kbytes");
		Renderer.Cursor = {0, Renderer.Cursor.y+16};
	
	for (uint64_t i = 0; i < 20; i++){
		void* pageadr = PgAllocr.requestpage();
		Renderer.print(0xfffffff, "Page "); Renderer.print(0xfffffff, tostr(i)); Renderer.print(0xfffffff, ": ");
		Renderer.print(0x00fffffff, h_tostr((uint64_t)pageadr));
		Renderer.Cursor = {0, Renderer.Cursor.y+16};
	}

    return; //Returns to EFI Shell
};