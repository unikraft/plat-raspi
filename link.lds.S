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
#include <common.lds.h>

#define RAM_BASE_ADDR	0x80000
#define RAM_END_ADDRESS	0x3E000000

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

	. = ALIGN(__PAGE_SIZE);
	_etext = .;

	//EXCEPTION_SECTIONS

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
	. = ALIGN(__PAGE_SIZE);

	/* Read-write data that is not initialized explicitly in code */
    .bss (NOLOAD) :
	{
        . = ALIGN(16);
        __bss_start = .;
		*(.bss)
		*(.bss.*)
        *(COMMON)
        __bss_end = .;
    }

	. = ALIGN(__PAGE_SIZE);
    _end = .;

	//DEBUG_SYMBOLS

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>3;
