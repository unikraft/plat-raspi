/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 * Copyright (c) 2018 Sergey Matyukevich
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

#include <raspi/sysregs.h>
#include <raspi/mm.h>
#include <raspi/mmu.h>
#include <uk/config.h>

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
	ldr		x0, =SCTLR_EL1_VALUE_MMU_DISABLED
	msr		sctlr_el1, x0

	ldr		x0, =SCTLR_EL2_VALUE
	msr		sctlr_el2, x0

// Disable coprocessor traps
	ldr		x0, =CPACR_EL1_VALUE
	msr		cpacr_el1, x0

	ldr		x0, =HCR_EL2_VALUE
	msr		hcr_el2, x0

	// Set the SPSR state to restore when returning from EL2 to EL1
	ldr		x0, =SPSR_EL2_VALUE
	msr		spsr_el2, x0

	adr		x0, el1_entry
	msr		elr_el2, x0

	ldr		x0, =0x40000040
	ldr		w14, [x0]
	orr		w14, w14, #8
	str		w14, [x0]

	eret

el1_entry:
	bl 	create_page_tables

	adrp	x0, _pagetables				
	msr		ttbr1_el1, x0		
	msr		ttbr0_el1, x0

	ldr		x0, =(TCR_VALUE)		
	msr		tcr_el1, x0

	ldr		x0, =(MAIR_VALUE)
	msr		mair_el1, x0


// Jump to C code, should not return
	ldr		x4, =_libraspiplat_entry
	ldr		x5, =SCTLR_EL1_VALUE_MMU_ENABLED
	msr		sctlr_el1, x5


#if CONFIG_RASPI_WATERMARK_STACK
watermark_stack_start:
    ldr     x1, =VA_START
    ldr     w2, =0x10000
watermark_stack_loop: 
	cbz     w2, watermark_stack_done
    str     x2, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, watermark_stack_loop
watermark_stack_done:
#endif


clear_bss_start:
	// Clear bss
	ldr     x1, =__bss_start
	ldr     w2, =__bss_size
clear_bss_loop:
	cbz     w2, clear_bss_done
	str     xzr, [x1], #8
	sub     w2, w2, #1
	cbnz    w2, clear_bss_loop
clear_bss_done:


    // Set the stack before our code
	msr		SPSel, #1
    ldr     x1, =_start
	mov		sp, x1


jump_to_C:
// Recover initial timer value
	mov		w0, w10
	mov		w1, w11
	mov		w2, w12
	mov		w3, w13
	//bl      _libraspiplat_entry
	br 		x4

	// As a failsafe, we also hang the main core
    b       hang





	.macro	create_pgd_entry, tbl, virt, tmp1, tmp2
	create_table_entry \tbl, \virt, PGD_SHIFT, \tmp1, \tmp2
	create_table_entry \tbl, \virt, PUD_SHIFT, \tmp1, \tmp2
	.endm

	.macro	create_table_entry, tbl, virt, shift, tmp1, tmp2
	lsr	\tmp1, \virt, #\shift
	and	\tmp1, \tmp1, #PTRS_PER_TABLE - 1			// table index
	add	\tmp2, \tbl, #PAGE_SIZE
	orr	\tmp2, \tmp2, #MM_TYPE_PAGE_TABLE	
	str	\tmp2, [\tbl, \tmp1, lsl #3]
	add	\tbl, \tbl, #PAGE_SIZE					// next level table page
	.endm

	.macro	create_block_map, tbl, phys, start, end, flags, tmp1
	lsr	\start, \start, #SECTION_SHIFT
	and	\start, \start, #PTRS_PER_TABLE - 1			// table index
	lsr	\end, \end, #SECTION_SHIFT
	and	\end, \end, #PTRS_PER_TABLE - 1				// table end index
	lsr	\phys, \phys, #SECTION_SHIFT
	mov	\tmp1, #\flags
	orr	\phys, \tmp1, \phys, lsl #SECTION_SHIFT			// table entry
9999:	str	\phys, [\tbl, \start, lsl #3]				// store the entry
	add	\start, \start, #1					// next entry
	add	\phys, \phys, #SECTION_SIZE				// next block
	cmp	\start, \end
	b.ls	9999b
	.endm

create_page_tables:
	mov	x29, x30						// save return address

	adrp	x0, _pagetables
	mov	x1, #PG_DIR_SIZE
	bl 	memzero

	adrp	x0, _pagetables
	mov		x1, #VA_START 
	create_pgd_entry x0, x1, x2, x3

	/* Mapping kernel and init stack*/
	mov 	x1, xzr											// start mapping from physical offset 0
	mov 	x2, #VA_START									// first virtual address
	ldr	x3, =(VA_START + DEVICE_BASE - SECTION_SIZE)		// last virtual address
	create_block_map x0, x1, x2, x3, MMU_FLAGS, x4

	/* Mapping device memory*/
	mov 	x1, #DEVICE_BASE					// start mapping from device base address 
	ldr 	x2, =(VA_START + DEVICE_BASE)				// first virtual address
	ldr	x3, =(VA_START + PHYS_MEMORY_SIZE - SECTION_SIZE)	// last virtual address
	create_block_map x0, x1, x2, x3, MMU_DEVICE_FLAGS, x4

	mov	x30, x29						// restore return address
	ret
