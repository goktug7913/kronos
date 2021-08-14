cd kernel
rm -r iso-root
rm Kronos.iso
rm -r lib
make setup
make kernel
make buildimg