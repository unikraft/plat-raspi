/*
 * MIT License
 *
 * Copyright (c) 2018, Sergey Matyukevich
 *           (c) 2020, Santiago Pagani <santiagopagani@gmail.com>
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

#include <raspi/entry.h>

	.macro handle_invalid_entry type
	kernel_entry
	mov	x0, #\type
	bl	show_invalid_entry_message
	b	err_hang
	.endm

	.macro	ventry	label
	.align	7
	b	\label
	.endm

	.macro	kernel_entry
	sub	sp, sp, #S_FRAME_SIZE
	stp	x0, x1, [sp, #16 * 0]
	stp	x2, x3, [sp, #16 * 1]
	stp	x4, x5, [sp, #16 * 2]
	stp	x6, x7, [sp, #16 * 3]
	stp	x8, x9, [sp, #16 * 4]
	stp	x10, x11, [sp, #16 * 5]
	stp	x12, x13, [sp, #16 * 6]
	stp	x14, x15, [sp, #16 * 7]
	stp	x16, x17, [sp, #16 * 8]
	stp	x18, x19, [sp, #16 * 9]
	stp	x20, x21, [sp, #16 * 10]
	stp	x22, x23, [sp, #16 * 11]
	stp	x24, x25, [sp, #16 * 12]
	stp	x26, x27, [sp, #16 * 13]
	stp	x28, x29, [sp, #16 * 14]
	str	x30, [sp, #16 * 15] 
	.endm

	.macro	kernel_exit
	ldp	x0, x1, [sp, #16 * 0]
	ldp	x2, x3, [sp, #16 * 1]
	ldp	x4, x5, [sp, #16 * 2]
	ldp	x6, x7, [sp, #16 * 3]
	ldp	x8, x9, [sp, #16 * 4]
	ldp	x10, x11, [sp, #16 * 5]
	ldp	x12, x13, [sp, #16 * 6]
	ldp	x14, x15, [sp, #16 * 7]
	ldp	x16, x17, [sp, #16 * 8]
	ldp	x18, x19, [sp, #16 * 9]
	ldp	x20, x21, [sp, #16 * 10]
	ldp	x22, x23, [sp, #16 * 11]
	ldp	x24, x25, [sp, #16 * 12]
	ldp	x26, x27, [sp, #16 * 13]
	ldp	x28, x29, [sp, #16 * 14]
	ldr	x30, [sp, #16 * 15] 
	add	sp, sp, #S_FRAME_SIZE		
	eret
	.endm


/*
 * Exception vectors.
 */
.align	11
.globl vectors_el3
vectors_el3:
	ventry	sync_invalid_el3t			// Synchronous EL3t
	ventry	irq_invalid_el3t			// IRQ EL3t
	ventry	fiq_invalid_el3t			// FIQ EL3t
	ventry	error_invalid_el3t			// Error EL3t

	ventry	sync_invalid_el3h			// Synchronous EL3h
	ventry	irq_invalid_el3h			// IRQ EL3h
	ventry	fiq_invalid_el3h			// FIQ EL3h
	ventry	error_invalid_el3h			// Error EL3h

	ventry	sync_invalid_el2_64			// Synchronous 64-bit EL2
	ventry	irq_invalid_el2_64			// IRQ 64-bit EL2
	ventry	fiq_invalid_el2_64			// FIQ 64-bit EL2
	ventry	error_invalid_el2_64		// Error 64-bit EL2

	ventry	sync_invalid_el2_32			// Synchronous 32-bit EL2
	ventry	irq_invalid_el2_32			// IRQ 32-bit EL2
	ventry	fiq_invalid_el2_32			// FIQ 32-bit EL2
	ventry	error_invalid_el2_32		// Error 32-bit EL2

.align	11
.globl vectors_el2
vectors_el2:
	ventry	sync_invalid_el2t			// Synchronous EL2t
	ventry	irq_invalid_el2t			// IRQ EL2t
	ventry	fiq_invalid_el2t			// FIQ EL2t
	ventry	error_invalid_el2t			// Error EL2t

	ventry	sync_invalid_el2h			// Synchronous EL2h
	ventry	irq_invalid_el2h			// IRQ EL2h
	ventry	fiq_invalid_el2h			// FIQ EL2h
	ventry	error_invalid_el2h			// Error EL2h

	ventry	sync_invalid_el1_64			// Synchronous 64-bit EL1
	ventry	irq_invalid_el1_64			// IRQ 64-bit EL1
	ventry	fiq_invalid_el1_64			// FIQ 64-bit EL1
	ventry	error_invalid_el1_64		// Error 64-bit EL1

	ventry	sync_invalid_el1_32			// Synchronous 32-bit EL1
	ventry	irq_invalid_el1_32			// IRQ 32-bit EL1
	ventry	fiq_invalid_el1_32			// FIQ 32-bit EL1
	ventry	error_invalid_el1_32		// Error 32-bit EL1

.align	11
.globl vectors_el1 
vectors_el1:
	ventry	sync_invalid_el1t			// Synchronous EL1t
	ventry	irq_invalid_el1t			// IRQ EL1t
	ventry	fiq_invalid_el1t			// FIQ EL1t
	ventry	error_invalid_el1t			// Error EL1t

	ventry	el1_sync					// Synchronous EL1h
	ventry	el1_irq						// IRQ EL1h
	ventry	fiq_invalid_el1h			// FIQ EL1h
	ventry	error_invalid_el1h			// Error EL1h

	ventry	sync_invalid_el0_64			// Synchronous 64-bit EL0
	ventry	irq_invalid_el0_64			// IRQ 64-bit EL0
	ventry	fiq_invalid_el0_64			// FIQ 64-bit EL0
	ventry	error_invalid_el0_64		// Error 64-bit EL0

	ventry	sync_invalid_el0_32			// Synchronous 32-bit EL0
	ventry	irq_invalid_el0_32			// IRQ 32-bit EL0
	ventry	fiq_invalid_el0_32			// FIQ 32-bit EL0
	ventry	error_invalid_el0_32		// Error 32-bit EL0

sync_invalid_el3t:
	handle_invalid_entry  SYNC_INVALID_EL3t

irq_invalid_el3t:
	handle_invalid_entry  IRQ_INVALID_EL3t

fiq_invalid_el3t:
	handle_invalid_entry  FIQ_INVALID_EL3t

error_invalid_el3t:
	handle_invalid_entry  ERROR_INVALID_EL3t

sync_invalid_el3h:
	handle_invalid_entry  SYNC_INVALID_EL3h

irq_invalid_el3h:
	handle_invalid_entry  IRQ_INVALID_EL3h

fiq_invalid_el3h:
	handle_invalid_entry  FIQ_INVALID_EL3h

error_invalid_el3h:
	handle_invalid_entry  ERROR_INVALID_EL3h

sync_invalid_el2_64:
	handle_invalid_entry  SYNC_INVALID_EL2_64

irq_invalid_el2_64:
	handle_invalid_entry  IRQ_INVALID_EL2_64

fiq_invalid_el2_64:
	handle_invalid_entry  FIQ_INVALID_EL2_64

error_invalid_el2_64:
	handle_invalid_entry  ERROR_INVALID_EL2_64

sync_invalid_el2_32:
	handle_invalid_entry  SYNC_INVALID_EL2_32

irq_invalid_el2_32:
	handle_invalid_entry  IRQ_INVALID_EL2_32

fiq_invalid_el2_32:
	handle_invalid_entry  FIQ_INVALID_EL2_32

error_invalid_el2_32:
	handle_invalid_entry  ERROR_INVALID_EL2_32

sync_invalid_el2t:
	handle_invalid_entry  SYNC_INVALID_EL2t

irq_invalid_el2t:
	handle_invalid_entry  IRQ_INVALID_EL2t

fiq_invalid_el2t:
	handle_invalid_entry  FIQ_INVALID_EL2t

error_invalid_el2t:
	handle_invalid_entry  ERROR_INVALID_EL2t

sync_invalid_el2h:
	handle_invalid_entry  SYNC_INVALID_EL2h

irq_invalid_el2h:
	handle_invalid_entry  IRQ_INVALID_EL2h

fiq_invalid_el2h:
	handle_invalid_entry  FIQ_INVALID_EL2h

error_invalid_el2h:
	handle_invalid_entry  ERROR_INVALID_EL2h

sync_invalid_el1_64:
	handle_invalid_entry  SYNC_INVALID_EL1_64

irq_invalid_el1_64:
	handle_invalid_entry  IRQ_INVALID_EL1_64

fiq_invalid_el1_64:
	handle_invalid_entry  FIQ_INVALID_EL1_64

error_invalid_el1_64:
	handle_invalid_entry  ERROR_INVALID_EL1_64

sync_invalid_el1_32:
	handle_invalid_entry  SYNC_INVALID_EL1_32

irq_invalid_el1_32:
	handle_invalid_entry  IRQ_INVALID_EL1_32

fiq_invalid_el1_32:
	handle_invalid_entry  FIQ_INVALID_EL1_32

error_invalid_el1_32:
	handle_invalid_entry  ERROR_INVALID_EL1_32

sync_invalid_el1t:
	handle_invalid_entry  SYNC_INVALID_EL1t

irq_invalid_el1t:
	handle_invalid_entry  IRQ_INVALID_EL1t

fiq_invalid_el1t:
	handle_invalid_entry  FIQ_INVALID_EL1t

error_invalid_el1t:
	handle_invalid_entry  ERROR_INVALID_EL1t

sync_invalid_el1h:
	handle_invalid_entry  SYNC_INVALID_EL1h

irq_invalid_el1h:
	handle_invalid_entry  IRQ_INVALID_EL1h

fiq_invalid_el1h:
	handle_invalid_entry  FIQ_INVALID_EL1h

error_invalid_el1h:
	handle_invalid_entry  ERROR_INVALID_EL1h

sync_invalid_el0_64:
	handle_invalid_entry  SYNC_INVALID_EL0_64

irq_invalid_el0_64:
	handle_invalid_entry  IRQ_INVALID_EL0_64

fiq_invalid_el0_64:
	handle_invalid_entry  FIQ_INVALID_EL0_64

error_invalid_el0_64:
	handle_invalid_entry  ERROR_INVALID_EL0_64

sync_invalid_el0_32:
	handle_invalid_entry  SYNC_INVALID_EL0_32

irq_invalid_el0_32:
	handle_invalid_entry  IRQ_INVALID_EL0_32

fiq_invalid_el0_32:
	handle_invalid_entry  FIQ_INVALID_EL0_32

error_invalid_el0_32:
	handle_invalid_entry  ERROR_INVALID_EL0_32

el1_sync:
	kernel_entry
	mrs	x0, ESR_EL1
	mrs	x1, FAR_EL1
	bl	show_invalid_entry_message_el1_sync
	b	err_hang

el1_irq:
	kernel_entry 
	bl	ukplat_irq_handle
	kernel_exit

.globl err_hang
err_hang: b err_hang
