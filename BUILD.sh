cd gnu-efi
make bootloader

cd ../kernel
make setup
make kernel
make buildimg