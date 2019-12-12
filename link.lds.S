/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */
#include <uk/arch/limits.h>
#include <uk/plat/common/common.lds.h>

#define RAM_BASE_ADDR	0x80000
//#define RAM_BASE_ADDR	0xFFFF000000080000

OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_libraspiplat_entry)

SECTIONS
{
    . = RAM_BASE_ADDR;

	/* Place DTB binary at the beginning of the RAM */
	_dtb = .;
	//. = . + DTB_RESERVED_SIZE;

	/* Code */
	_text = .;
	.text :
	{
		KEEP(*(.text.boot))
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t*)
	}
	_etext = .;

	. = ALIGN(__PAGE_SIZE);
	EXCEPTION_SECTIONS

	CTORTAB_SECTION

	INITTAB_SECTION

	/* Read-only data */
	. = ALIGN(__PAGE_SIZE);
	_rodata = .;
	.rodata :
	{
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r*)
	}
	_erodata = .;

	/* Constructor tables (read-only) */
	. = ALIGN(0x8);
	_ctors = .;
	.preinit_array : {
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	}

	. = ALIGN(0x8);
	.init_array : {
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
		KEEP (*(.init_array .ctors))
		PROVIDE_HIDDEN (__init_array_end = .);
	}
	_ectors = .;

	. = ALIGN(__PAGE_SIZE);
	TLS_SECTIONS

	/* Read-write data that is initialized explicitly in code */
	_data = .;
	.data :
	{
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
	}
	_edata = .;

	/* Read-write data that is not initialized explicitly in code */
	. = ALIGN(__PAGE_SIZE);
	__bss_start = .;
    .bss (NOLOAD) :
	{
		*(.bss)
		*(.bss.*)
        *(COMMON)
    }
	__bss_end = .;

	/* From https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson06/rpi-os.md:
	 * We need just one PGD and one PUD to map the whole RPi memory, and both PGD and PUD will contain a single descriptor.
	 * If we have a single PUD entry there also must be a single PMD table, to which this entry will point.
	 * Single PMD entry covers 2 MB, there are 512 items in a PMD, so in total the whole PMD table covers the same 1 GB of
	 * memory that is covered by a single PUD descriptor.
	 * We need to map 1 GB region of memory, which is a multiple of 2 MB, therefore we can use section mapping, and hence we
	 * don't need PTE at all.
	 * Finally, that means we need three 4 kB pages for the page tables: one for PGD, PUD and PMD.
	 */
	. = ALIGN(__PAGE_SIZE);
	_pagetables = .;
	.pagetables (NOLOAD) :
	{
		. += 3 * __PAGE_SIZE;
	}

	_end = .;
	

	. = ALIGN(__PAGE_SIZE);
	.comment       0 : { *(.comment) }
	DEBUG_SYMBOLS

   /DISCARD/ : { *(.gnu*) *(.note*) }
}
__bss_size = (__bss_end - __bss_start)>>3;
