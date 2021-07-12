set OSNAME=Kronos
set BUILDDIR=%0/../
set OVMFDIR=%0/../../OVMFbin

set BUILDDIR=%BUILDDIR:"=%
set OVMFDIR=%OVMFDIR:"=%
qemu-system-x86_64 -drive file=%BUILDDIR%/%OSNAME%.iso -m 128M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file=%OVMFDIR%/OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=%OVMFDIR%/OVMF_VARS-pure-efi.fd -net none -s -S