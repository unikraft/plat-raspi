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

#include <stdlib.h>
#include <uk/assert.h>
#include <uk/plat/time.h>
#include <uk/plat/lcpu.h>
#include <uk/plat/irq.h>
#include <uk/bitops.h>
#include <uk/essentials.h>
#include <uk/plat/common/cpu.h>
#include <uk/plat/common/irq.h>
#include <arm/time.h>
#include <raspi/time.h>
#include <raspi/irq.h>

#define RASPI_ARM_SIDE_TIMER_LOAD_INIT	(0x00FFFFFF)

static uint32_t timer_irq_delay;

void generic_timer_mask_irq(void)
{
	set_el0(cntv_ctl, get_el0(cntv_ctl) | GT_TIMER_MASK_IRQ);

	/* Ensure the write of sys register is visible */
	isb();
}

void generic_timer_unmask_irq(void)
{
	set_el0(cntv_ctl, get_el0(cntv_ctl) & ~GT_TIMER_MASK_IRQ);

	/* Ensure the write of sys register is visible */
	isb();
}

uint32_t generic_timer_get_frequency(int fdt_timer __unused)
{
	return get_el0(cntfrq);
}

unsigned long sched_have_pending_events;

void time_block_until(__snsec until)
{
	while ((__snsec) ukplat_monotonic_clock() < until) {
		generic_timer_cpu_block_until(until);
	}
}

/* must be called before interrupts are enabled */
void ukplat_time_init(void)
{
	int rc;

	/*
	 * Monotonic time begins at boot_ticks (first read of counter
	 * before calibration).
	 */
	generic_timer_update_boot_ticks();

	/* Currently, we only support 1 timer per system */
	rc = generic_timer_init(0);
	if (rc < 0)
		UK_CRASH("Failed to initialize platform time\n");

	rc = ukplat_irq_register(IRQ_ID_ARM_GENERIC_TIMER, generic_timer_irq_handler, NULL);
	if (rc < 0)
		UK_CRASH("Failed to register timer interrupt handler\n");

	/*
	 * Mask IRQ before scheduler start working. Otherwise we will get
	 * unexpected timer interrupts when system is booting.
	 */
	generic_timer_mask_irq();

	/* Enable timer */
	generic_timer_enable();
}

static void raspi_arm_side_timer_init(void)
{
	*RASPI_ARM_SIDE_TIMER_CTL = RASPI_ARM_SIDE_TIMER_CTL_ENABLE_BIT | RASPI_ARM_SIDE_TIMER_CTL_BITS_BIT;
	*RASPI_ARM_SIDE_TIMER_PREDIVIDER = 0;
	*RASPI_ARM_SIDE_TIMER_LOAD = RASPI_ARM_SIDE_TIMER_LOAD_INIT;
}

static int handle_raspi_side_timer_irq(void *arg __unused)
{
	uint64_t timerValue1 = raspi_arm_side_timer_get_value();
	uint64_t timerValue2 = raspi_arm_side_timer_get_value();
	raspi_arm_side_timer_irq_disable();
	raspi_arm_side_timer_irq_clear();

	// The counter is decreasing, so to get the delay of the IRQ response we substract the value of the timer at the entry of this
	// function from the timer load value. Further, to account for the time needed to sample the timer, we take a second sample
	// and also substract the difference beween the two points
	timer_irq_delay = (raspi_arm_side_timer_get_load() - timerValue1) - (timerValue1 - timerValue2);

	return 1;
}

void raspi_irq_delay_measurements_init(void)
{
	int rc;

	raspi_arm_side_timer_init();

	rc = ukplat_irq_register(IRQ_ID_RASPI_ARM_SIDE_TIMER, handle_raspi_side_timer_irq, NULL);
	if (rc < 0)
		UK_CRASH("Failed to register timer interrupt handler\n");
}

/**
 * Get System Timer's counter
 */
uint64_t get_system_timer(void)
{
	uint32_t h, l;
	// we must read MMIO area as two separate 32 bit reads
	h = *RASPI_SYS_TIMER_CHI;
	l = *RASPI_SYS_TIMER_CLO;
	// we have to repeat it if high word changed during read
	if (h != *RASPI_SYS_TIMER_CHI)
	{
		h = *RASPI_SYS_TIMER_CHI;
		l = *RASPI_SYS_TIMER_CLO;
	}
	// compose long int value
	return ((uint64_t)h << 32) | l;
}

uint32_t get_timer_irq_delay(void)
{
	return timer_irq_delay;
}

void reset_timer_irq_delay(void)
{
	timer_irq_delay = 0;
}
