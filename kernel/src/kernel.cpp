#include "stivale2.h"
#include <stdint.h>
#include <stddef.h>
#include "renderer.h"
#include "psf.h"

// We need to tell the stivale bootloader where we want our stack to be.
// We are going to allocate our stack as an uninitialised array in .bss.
static uint8_t stack[4096];
 
// stivale2 uses a linked list of tags for both communicating TO the
// bootloader, or receiving info FROM it. More information about these tags
// is found in the stivale2 specification.
 
// stivale2 offers a runtime terminal service which can be ditched at any
// time, but it provides an easy way to print out to graphical terminal,
// especially during early boot.
// Read the notes about the requirements for using this feature below this
// code block.
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    // All tags need to begin with an identifier and a pointer to the next tag.
    .tag = {
        // Identification constant defined in stivale2.h and the specification.
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        // If next is 0, it marks the end of the linked list of header tags.
        .next = 0
    },
    // The terminal header tag possesses a flags field, leave it as 0 for now
    // as it is unused.
    .flags = 0
};
 
// We are now going to define a framebuffer header tag, which is mandatory when
// using the stivale2 terminal.
// This tag tells the bootloader that we want a graphical framebuffer instead
// of a CGA-compatible text mode. Omitting this tag will make the bootloader
// default to text mode, if available.
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    // Same as above.
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        // Instead of 0, we now point to the previous header tag. The order in
        // which header tags are linked does not matter.
        .next = (uint64_t)&terminal_hdr_tag
    },
    // We set all the framebuffer specifics to 0 as we want the bootloader
    // to pick the best it can.
    .framebuffer_width  = 1920,
    .framebuffer_height = 1080,
    .framebuffer_bpp    = 24
};
 
// The stivale2 specification says we need to define a "header structure".
// This structure needs to reside in the .stivale2hdr ELF section in order
// for the bootloader to find it. We use this __attribute__ directive to
// tell the compiler to put the following structure in said section.
__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
    // The entry_point member is used to specify an alternative entry
    // point that the bootloader should jump to instead of the executable's
    // ELF entry point. We do not care about that so we leave it zeroed.
    .entry_point = 0,
    // Let's tell the bootloader where our stack is.
    // We need to add the sizeof(stack) since in x86(_64) the stack grows
    // downwards.
    .stack = (uintptr_t)stack + sizeof(stack),
    // Bit 1, if set, causes the bootloader to return to us pointers in the
    // higher half, which we likely want.
    .flags = (1 << 1),
    // This header structure is the root of the linked list of header tags and
    // points to the first one in the linked list.
    .tags = (uintptr_t)&framebuffer_hdr_tag
};

extern "C" void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (stivale2_tag *)stivale2_struct->tags;
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
        current_tag = (stivale2_tag *)current_tag->next;
    }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


Renderer r = Renderer(NULL, 0); //Stub

extern "C" [[noreturn]] void _start(struct stivale2_struct *bootInfo){
    
    stivale2_struct_tag_modules* modules = (stivale2_struct_tag_modules *) stivale2_get_tag(bootInfo,STIVALE2_STRUCT_TAG_MODULES_ID);
    stivale2_struct_tag_memmap* memmap = (stivale2_struct_tag_memmap *) stivale2_get_tag(bootInfo, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    stivale2_struct_tag_framebuffer *sti_framebuffer = (stivale2_struct_tag_framebuffer*) stivale2_get_tag(bootInfo, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

    stivale2_module* fontModule = modules->modules;
    //PSF_font font = *(PSF_font*)fontModule->begin;
    uint64_t* font = (uint64_t*)fontModule->begin;

    r = Renderer(sti_framebuffer, font);
    kRenderer = &r;

    kRenderer->Init();
    kRenderer->SetBgColor(0x002338);
    kRenderer->color = 0xffb0fc;

    kRenderer->Clear();

    kRenderer->putc('K',100, 100);
    kRenderer->putc('r',110, 100);
    kRenderer->putc('o',120, 100);
    kRenderer->putc('n',130, 100);
    kRenderer->putc('o',140, 100);
    kRenderer->putc('s',150, 100);

        for (size_t x = 0; x < 200; x++){
            kRenderer->PlotPixel(x,5,0xFFFFFFF);
            kRenderer->PlotPixel(x,6,0xFFFFFFF);
            kRenderer->PlotPixel(x,7,0xFFFFFFF);
        }
    while(true){};
}