@echo -off
mode 80 25

cls
if exist .\EFI\BOOT\BOOTX64.EFI then
 .\EFI\BOOT\BOOTX64.EFI
 goto END
endif

if exist fs0:\EFI\boot\BOOTX64.EFI then
 fs0:
 echo Found Kronos Bootloader on fs0:
 \EFI\BOOT\BOOTX64.EFI
 goto END
endif

if exist fs1:\EFI\BOOT\BOOTX64.EFI then
 fs1:
 echo Found Kronos Bootloader on fs1:
 \EFI\BOOT\BOOTX64.EFI
 goto END
endif

if exist fs2:\EFI\BOOT\BOOTX64.EFI then
 fs2:
 echo Found Kronos Bootloader on fs2:
 \EFI\BOOT\BOOTX64.EFI
 goto END
endif

if exist fs3:\EFI\BOOT\BOOTX64.EFI then
 fs3:
 echo Found Kronos Bootloader on fs3:
 \EFI\BOOT\BOOTX64.EFI
 goto END
endif

if exist fs4:\EFI\BOOT\BOOTX64.EFI then
 fs4:
 echo Found Kronos Bootloader on fs4:
 \EFI\BOOT\BOOTX64.EFI
 goto END
endif

if exist fs5:\EFI\BOOT\BOOTX64.EFI then
 fs5:
 echo Found Kronos Bootloader on fs5:
 \EFI\BOOT\BOOTX64.EFI
 goto END
endif

if exist fs6:\EFI\BOOT\BOOTX64.EFI then
 fs6:
 echo Found Kronos Bootloader on fs6:
 \EFI\BOOT\BOOTX64.EFI
 goto END
endif

if exist fs7:\EFI\BOOT\BOOTX64.EFI then
 fs7:
 echo Found Kronos Bootloader on fs7:
 \EFI\BOOT\BOOTX64.EFI
 goto END
endif

 echo "Unable to find Limine Bootloader".
 
:END
