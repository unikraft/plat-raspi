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

#include "raspi/sysregs.h"

.section ".text.boot"

.global _start

_start:
    // read cpu id, stop slave cores
	ldr		x0, =0x3F003000
	ldr		w10, [x0, #4]
	ldr		w11, [x0, #8]
	ldr		w12, [x0, #4]
	ldr		w13, [x0, #8]
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, master

// If the cpu id is > 0, hang here
hang:  wfe
    b       hang

// Continue if cpu id == 0
master:
	//ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	//msr	sctlr_el1, x0		

	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	//ldr	x0, =SCR_VALUE
	//msr	scr_el3, x0

	//ldr	x0, =SPSR_VALUE
	//msr	spsr_el3, x0

	adr		x0, el1_entry
	msr		elr_el2, x0
	mov		x30, x0

	//eret

el1_entry:
    // Set the stack before our code
    ldr     x1, =_start
    mov     sp, x1

    // Clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
clear_bss: 
	cbz     w2, jump_to_C
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, clear_bss

// Jump to C code, should not return
jump_to_C:
	mov		w0, w10
	mov		w1, w11
	mov		w2, w12
	mov		w3, w13
	bl      _libraspiplat_entry

	// As a failsafe, we also hang the main core
    b       hang
