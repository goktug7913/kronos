OSNAME = Kronos

LIMINE = ../limine
OVMFDIR = ../OVMFbin
LDS = kernel.ld
CC = gcc
ASMC = nasm
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone -O0 -g -I/home/gktdev/Kronos/commonlib -fpermissive
ASMFLAGS =
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

# Internal link flags that should not be changed by the user.
INTERNALLDFLAGS :=     \
		-fno-pic -fpie \
		-Wl,-static,-pie,--no-dynamic-linker,-ztext \
		-static-pie    \
		-nostdlib      \
		-Tlinker.ld    \
		-z max-page-size=0x1000
 
# Internal C flags that should not be changed by the user.
INTERNALCFLAGS  :=           \
		-I.                  \
		-std=gnu11           \
		-ffreestanding       \
		-fno-stack-protector \
		-fno-pic -fpie       \
		-mno-80387           \
		-mno-mmx             \
		-mno-3dnow           \
		-mno-sse             \
		-mno-sse2            \
		-mno-red-zone

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
BOOTEFI := $(LIMINE)/BOOTX64.EFI

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)

OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))

DIRS = $(wildcard $(SRCDIR)/*)

kernel: $(OBJS) link

$(OBJDIR)/interrupts/interrupts.o: $(SRCDIR)/interrupts/interrupts.cpp
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -I/home/gktdev/Kronos/commonlib -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@
	
$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@ echo !==== COMPILING $^
	@ mkdir -p $(@D)
	$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@
	
link:
	@ echo !==== LINKING
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/stivale2.elf $(OBJS)

setup:
	@mkdir $(OBJDIR)

buildimg:
	
	mkdir -p iso_root
	
	cp -v $(BUILDDIR)/stivale2.elf $(BUILDDIR)/limine.cfg $(BUILDDIR)/zap-light16.psf\
		limine/limine-cd.bin limine/limine-eltorito-efi.bin iso_root/
	
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o Kronos.iso
 
# Install Limine stage 1 and 2 for legacy BIOS boot.
	./limine/limine-install Kronos.iso

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none