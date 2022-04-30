# Raspberry Pi 3B+ platform

## Integration steps

In order to compile an application that will run on the Raspberry Pi 3B+, please:
- clone this repository into **unikraft/plat/** using following command

```
cd unkiraft/plat
git clone --branch spagani-devel git@github.com:unikraft/plat-raspi.git raspi
```
- update **unikraft/plat/Makefile.uk** to register the Raspberry Pi platform we just cloned.
```
echo '$(eval $(call _import_lib,$(UK_PLAT_BASE)/raspi))' >> Makefile.uk
```
- run `make menuconfig` in unikraft directory
- select/deselect the following options in the make menuconfig:
	- Architecture Selection --> Architecture --> select: Armv8 compatible (64 bits)
	- Architecture Selection --> Processor Optimization --> select: Generic Armv8 Cortex A53
	- Architecture Selection --> deselect: Workaround for Cortex-A73 erratum
	- Platform Configuration --> select: Raspberry Pi 3B+
	- Library Configuration --> ukboot --> deselect: Show Unikraft banner (this is to minimize boot time)
	- Build Options --> select: Drop unused functions and data

Once built, in a blank SD card formated to FAT32, copy the generated kernel8.img Unikraft image (without changing the name of the image file, as otherwise the Raspberry Pi's bootloader will not recongize it), plus the four files that are inside the plat/raspi/bootfiles folder (i.e., bootcode.bin, config.txt, fixup.dat, and start.elf).
