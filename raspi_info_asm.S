/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Authors: Santiago Pagani <santiagopagani@gmail.com>
 *
 * Copyright (c) 2020, NEC Laboratories Europe GmbH, NEC Corporation.
 *                     All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THIS HEADER MAY NOT BE EXTRACTED OR MODIFIED IN ANY WAY.
 */
 
.globl get_sp
get_sp:
    mov		x0, sp
    ret

.globl get_sp_start
get_sp_start:
    ldr     x1, =_start
    ret

.globl get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret

.globl get_spsel
get_spsel:
    mrs x0, SPSel
    ret

.globl get_sctlr_el1
get_sctlr_el1:
    mrs x0, sctlr_el1
    ret

.globl get_unikraft_text_size
get_unikraft_text_size:
	ldr x0, =_text
	ldr x1, =_ectors
	sub x0, x1, x0
	ret

.globl get_unikraft_data_size
get_unikraft_data_size:
	ldr x0, =_data
	ldr x1, =_edata
	sub x0, x1, x0
	ldr x1, =_tls_start
	ldr x2, =_etdata
	sub x1, x2, x1
	add x0, x0, x1
	ret

.globl get_unikraft_bss_size
get_unikraft_bss_size:
	ldr x0, =__bss_start
	ldr x1, =__bss_end
	sub x0, x1, x0
	ldr x1, =_etdata
	ldr x2, =_tls_end
	sub x1, x2, x1
	add x0, x0, x1
	ret
