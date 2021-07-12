OUTPUT_FORMAT(elf64-x86-64)

/* Tell the linker that we want the symbol _start to be our entry point */
ENTRY(_start)
 
SECTIONS
{
    /* We wanna be placed in the higher half, 2MiB above 0 in physical memory. */
    /* Since we are going to use PIE, this is just the base load address, but the */
    /* bootloader will be able to relocate us as it sees fit. */
    . = 0xffffffff80200000;
 
    /* We place the .stivale2hdr section containing the header in its own section, */
    /* and we use the KEEP directive on it to make sure it doesn't get discarded. */
    .stivale2hdr : 
	{
        KEEP(*(.stivale2hdr))
    }

	_KernelStart = .;
	.text : ALIGN(0x1000)
	{
		*(.text)
	}
	.data : ALIGN(0x1000)
	{
		*(.data)
		*(COMMON)
		_bssStart = .;
		*(.bss)
		_bssEnd = .;
	}
	.rodata : ALIGN(0x1000)
	{
		*(.rodata)
	}
	.bss : ALIGN(0x1000)
	{
	}
	_KernelEnd = .;
}