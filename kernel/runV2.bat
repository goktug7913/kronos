set OSNAME=Kronos
set BUILDDIR=%0/../
set OVMFDIR=%0/../../OVMFbin

set BUILDDIR=%BUILDDIR:"=%
set OVMFDIR=%OVMFDIR:"=%
qemu-system-x86_64 -drive file=%BUILDDIR%/%OSNAME%.iso -m 512 -cpu qemu64 -bios %OVMFDIR%/OVMF.fd -M q35 -s -S -no-reboot -no-shutdown