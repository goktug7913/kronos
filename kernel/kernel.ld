OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
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