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

#ifndef __RASPI_TIME_H__
#define __RASPI_TIME_H__

#include "sysregs.h"
#include <stdint.h>

#define RASPI_SYS_TIMER_BASE		MMIO_BASE
#define RASPI_SYS_TIMER_CS			((volatile uint32_t *)(RASPI_SYS_TIMER_BASE+0x00003000))
#define RASPI_SYS_TIMER_CLO			((volatile uint32_t *)(RASPI_SYS_TIMER_BASE+0x00003004))
#define RASPI_SYS_TIMER_CHI			((volatile uint32_t *)(RASPI_SYS_TIMER_BASE+0x00003008))
#define RASPI_SYS_TIMER_C0			((volatile uint32_t *)(RASPI_SYS_TIMER_BASE+0x0000300C))
#define RASPI_SYS_TIMER_C1			((volatile uint32_t *)(RASPI_SYS_TIMER_BASE+0x00003010))
#define RASPI_SYS_TIMER_C2			((volatile uint32_t *)(RASPI_SYS_TIMER_BASE+0x00003014))
#define RASPI_SYS_TIMER_C3			((volatile uint32_t *)(RASPI_SYS_TIMER_BASE+0x00003018))

#define RASPI_SYS_TIMER_CS_M0		(1 << 0)
#define RASPI_SYS_TIMER_CS_M1		(1 << 1)
#define RASPI_SYS_TIMER_CS_M2		(1 << 2)
#define RASPI_SYS_TIMER_CS_M3		(1 << 3)


#define RASPI_ARM_TIMER_BASE		(MMIO_BASE + 0x1000000)
#define RASPI_ARM_C0_TIMER_IRQ_CTL	((volatile uint32_t *)(RASPI_ARM_TIMER_BASE+0x40))
#define RASPI_ARM_C0_TIMER_IRQ_CTL_CNTVIRQ_BIT		(1 << 3)


#define RASPI_ARM_SIDE_TIMER_BASE		(MMIO_BASE + 0xB000)
#define RASPI_ARM_SIDE_TIMER_LOAD		((volatile uint32_t *)(RASPI_ARM_SIDE_TIMER_BASE+0x400))
#define RASPI_ARM_SIDE_TIMER_VALUE		((volatile uint32_t *)(RASPI_ARM_SIDE_TIMER_BASE+0x404))
#define RASPI_ARM_SIDE_TIMER_CTL		((volatile uint32_t *)(RASPI_ARM_SIDE_TIMER_BASE+0x408))
#define RASPI_ARM_SIDE_TIMER_IRQ_CLEAR	((volatile uint32_t *)(RASPI_ARM_SIDE_TIMER_BASE+0x40C))
#define RASPI_ARM_SIDE_TIMER_RAW_IRQ	((volatile uint32_t *)(RASPI_ARM_SIDE_TIMER_BASE+0x410))
#define RASPI_ARM_SIDE_TIMER_MASKED_IRQ	((volatile uint32_t *)(RASPI_ARM_SIDE_TIMER_BASE+0x414))
#define RASPI_ARM_SIDE_TIMER_RELOAD		((volatile uint32_t *)(RASPI_ARM_SIDE_TIMER_BASE+0x418))
#define RASPI_ARM_SIDE_TIMER_PREDIVIDER	((volatile uint32_t *)(RASPI_ARM_SIDE_TIMER_BASE+0x41C))
#define RASPI_ARM_SIDE_TIMER_FREE		((volatile uint32_t *)(RASPI_ARM_SIDE_TIMER_BASE+0x420))
#define RASPI_ARM_SIDE_TIMER_CTL_FREE_COUNTER_BIT	(1 << 9)
#define RASPI_ARM_SIDE_TIMER_CTL_ENABLE_BIT			(1 << 7)
#define RASPI_ARM_SIDE_TIMER_CTL_IRQ_BIT			(1 << 5)
#define RASPI_ARM_SIDE_TIMER_CTL_BITS_BIT			(1 << 1)


#define raspi_arm_side_timer_get_value()	(*RASPI_ARM_SIDE_TIMER_VALUE)
#define raspi_arm_side_timer_get_load()		(*RASPI_ARM_SIDE_TIMER_LOAD)
#define raspi_arm_side_timer_irq_enable()	{*RASPI_ARM_SIDE_TIMER_CTL = *RASPI_ARM_SIDE_TIMER_CTL | RASPI_ARM_SIDE_TIMER_CTL_IRQ_BIT; }
#define raspi_arm_side_timer_irq_disable()	{*RASPI_ARM_SIDE_TIMER_CTL = *RASPI_ARM_SIDE_TIMER_CTL & ~RASPI_ARM_SIDE_TIMER_CTL_IRQ_BIT; }
#define raspi_arm_side_timer_irq_clear()	{*RASPI_ARM_SIDE_TIMER_IRQ_CLEAR = 1; }
#define raspi_arm_side_timer_irq_triggered() (*RASPI_ARM_SIDE_TIMER_MASKED_IRQ & 1)


void raspi_irq_delay_measurements_init(void);
uint64_t get_system_timer(void);
uint32_t get_timer_irq_delay(void);
void reset_timer_irq_delay(void);

#endif /* __RASPI_TIME_H__ */