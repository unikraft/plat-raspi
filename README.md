# raspi

Raspberry Pi 3B+ platform

In order to compile an application that will run on the Raspberry Pi 3B+, please
select/deselect the following options in the make menuconfig:
- Architecture Selection --> Architecture --> select: Arrmv8 compatible (64 bits)
- Architecture Selection --> Processor Optimization --> select: Generic Armv8 Cortex A53
- Architecture Selection --> deselect: Workaround for Cortex-A73 erratum
- Platform Configuration --> select: Raspberry Pi 3B+
- Library Configuration --> ukboot --> deselect: Show Unikraft banner (this is to minimize boot time)
- Build Options --> select: Drop unused functions and data

Once built, in a blank SD card formated to FAT32, copy the generated kernel8.img Unikraft image (without changing
the name of the image file, as otherwise the Raspberry Pi's bootloader will not recongize it), plus the four files
that are inside the plats/raspi/bootfiles folder (i.e., bootcode.bin, config.txt, fixup.dat, and start.elf).
