#include <efi.h>
#include <efilib.h>
#include <elf.h>

#include "PSF.h"
#include "framebuffer.h"
#include "bootnfo.h"

Framebuffer framebuffer;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable){
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	if (Directory == NULL){
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS){
		return NULL;
	}
	return LoadedFile;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

PSF1_FONT* LoadPSF(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable){

	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if (font == NULL) return NULL;

	PSF1_HEADER* fontheader;

	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&fontheader); //Allocate memory for font
	UINTN size = sizeof(PSF1_HEADER);
	font->Read(font, &size, fontheader);

	if (fontheader->magic[0] != PSF1_MAGIC0 || fontheader->magic[1] != PSF1_MAGIC1) {
		return NULL;
	}

    UINTN glyphBufSize = fontheader->charsize *256;
    if (fontheader->mode == 1) { //Check if the glyphs are 512 mode
        glyphBufSize = fontheader->charsize * 512;
    }

    void* glyphbuf;
    {
        font->SetPosition(font, sizeof(PSF1_HEADER));
        SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufSize, (void**)&glyphbuf);
        font->Read(font, &glyphBufSize, glyphbuf);
    }

    PSF1_FONT* LoadedFont;
    SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&LoadedFont);
    LoadedFont->psf1_header = fontheader;
    LoadedFont->glyphBuf = glyphbuf;

    return LoadedFont;

}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

Framebuffer* InitializeGOP(){

	// INITIALIZES AND RETURNS ADDRESS OF THE GOP FRAMEBUFFER

	EFI_GUID GopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID; //Shorten GOP GUID

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
  	UINTN SizeOfInfo;

	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop; //Init GOP
	EFI_STATUS status;

	status = uefi_call_wrapper(BS->LocateProtocol, 3, &GopGuid, NULL, (void**)&gop);

	if (EFI_ERROR(status)){
		Print(L"Could not Locate GOP\n\r");
		return NULL;
	}

	else {Print(L"Located GOP\n\r");}
	
	//Query Current GOP Mode
	status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
	if (status == EFI_NOT_STARTED){
    	status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);

  		if(EFI_ERROR(status)) {
    		Print(L"Unable to get current GOP mode\r\n");
  		}	
    	framebuffer.NativeMode = gop->Mode->Mode;
	}

	//Query GOP Mode count
	framebuffer.ModeCount = gop->Mode->MaxMode;

	uint32_t setmode = framebuffer.NativeMode; // GOP Mode to set

	//Print All Modes and catch 1920x1080
	for (uint32_t i = 0; i < framebuffer.ModeCount; i++) {
  		status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo, &info);
		Print(L"mode %03d: X %d Y %d fmt %x%s\r\n",
		i,
		info->HorizontalResolution,
		info->VerticalResolution,
		info->PixelFormat,
		i == framebuffer.NativeMode ? "(current)" : ""
		);

		if (info->HorizontalResolution == 1920 && info->VerticalResolution == 1080){
			setmode = i;
		}
		
	}	

	//SET GOP MODE PROTOTYPE
	
	status = uefi_call_wrapper(gop->SetMode, 2, gop, setmode);
  	if(EFI_ERROR(status)) {
    	Print(L"Unable to set mode %03d\r\n", setmode);
	}
	
	//Get and set Framebuffer Core
	framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;

	//Get and set resolution from GOP
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return &framebuffer;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

int memcmp(const void* aptr, const void* bptr, size_t n){
	const unsigned char* a = aptr, *b = bptr;
	for (size_t i = 0; i < n; i++){
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

void memset(void* start, uint8_t value, uint64_t num){
    	for (uint64_t i = 0; i < num; i++){
        	*(uint8_t*)((uint64_t)start + i) = value;
    	}
	}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	Print(L"-----------------\n\r");
	Print(L"Kronos Bootloader\n\r");
	Print(L"\n\r");

	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);
	if (Kernel == NULL){
		Print(L"Failed To Load Kernel\n\r");
	}
	else{
		Print(L"Kernel Found...\n\r");
	}

	Elf64_Ehdr header;
{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);
	}

	if (
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	)
	{
		Print(L"Kernel Is Corrupted!\r\n");
	}
	else
	{
		Print(L"Kernel Header Verified.\r\n");
	}

	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	for (
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
	)
	{
		switch (phdr->p_type){
			case PT_LOAD:
			{
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

				Kernel->SetPosition(Kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				Kernel->Read(Kernel, &size, (void*)segment);
				break;
			}
		}
	}

	Print(L"Kernel Ready!\n\r\n\r");
	
	//Initialize Framebuffer
	Framebuffer* KronosFramebuffer = InitializeGOP();

	Print(L"Current GOP Mode: %d\n\r", 		KronosFramebuffer->NativeMode);
	Print(L"GOP Addr: 0x%x\n\r", 			KronosFramebuffer->BaseAddress);
	Print(L"size: 0x%x\n\r", 				KronosFramebuffer->BufferSize); 
	Print(L"Horizontal Resolution: %d\n\r", KronosFramebuffer->Width);
	Print(L"Vertical Resolution: %d\n\r", 	KronosFramebuffer->Height);
	Print(L"Scanline Pixel Length: %d\n\r", KronosFramebuffer->PixelsPerScanLine);
	Print(L"\n\r\n\r");
	
	PSF1_FONT* newFont = LoadPSF(NULL, L"zap-light18.psf", ImageHandle, SystemTable);

	if (newFont == NULL){
		Print(L"Font is not valid, or failed to load\r\n");
	}else{
		Print(L"Font Loaded: 'Zap-Light' Char Size: %d\n\r", newFont->psf1_header->charsize);
	}
	
	EFI_MEMORY_DESCRIPTOR* MemMap = NULL;
	UINTN MapSize, MapKey;
	UINTN DescSize;
	UINT32 DescVersion;
	{
		SystemTable->BootServices->GetMemoryMap(&MapSize, MemMap, &MapKey, &DescSize, &DescVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&MemMap);
		SystemTable->BootServices->GetMemoryMap(&MapSize, MemMap, &MapKey, &DescSize, &DescVersion);
	}

	//Declaration of kernel handoff function
	void (*KernelStart)(BootInfo*) = ((__attribute__((sysv_abi)) void (*)(BootInfo*) ) header.e_entry);
	
	BootInfo bootinfo;
	
	bootinfo.framebuffer = KronosFramebuffer;
	bootinfo.psf1_Font = newFont;
	bootinfo.mMap = MemMap;
	bootinfo.mMapSize = MapSize;
	bootinfo.mMapDescSize = DescSize;

	SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);


	//Zerofill .bss
	extern _bssStart;
	extern _bssEnd;
	uint64_t bssSize = (uint64_t)&_bssEnd - (uint64_t)&_bssStart;
	memset(_bssStart, 0, bssSize);

	//Call the kernel
	KernelStart(&bootinfo);

	return EFI_SUCCESS; // Exit the UEFI application
}
