/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Authors: Wei Chen <Wei.Chen@arm.com>
 *
 * Copyright (c) 2018, Arm Ltd. All rights reserved.
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
#include <uk/essentials.h>
#include <cpu.h>
#include <irq.h>
#include <arm/time.h>

#include <raspi/irq.h>
#include <raspi/time.h>
#include <uk/print.h>

/* How many nanoseconds per microsecond */
#define NSEC_PER_USEC		(1000)
#define RASPI_FREQUENCY		(250000000)
#define RASPI_ARM_SIDE_TIMER_LOAD_INIT	(0x00FFFFFF)

static __u32 timer_irq_delay;

void raspi_arm_side_timer_init(void)
{
	*RASPI_ARM_SIDE_TIMER_CTL = RASPI_ARM_SIDE_TIMER_CTL_ENABLE_BIT | RASPI_ARM_SIDE_TIMER_CTL_BITS_BIT;
	*RASPI_ARM_SIDE_TIMER_PREDIVIDER = 0;
	*ENABLE_BASIC_IRQS = 0x01;

	*RASPI_ARM_SIDE_TIMER_LOAD = RASPI_ARM_SIDE_TIMER_LOAD_INIT;
	raspi_arm_side_timer_irq_clear();
	raspi_arm_side_timer_irq_enable();
}

int handle_timer_irq(void *arg __unused)
{
	__u64 timerValue1 = raspi_arm_side_timer_get_value();
	__u64 timerValue2 = raspi_arm_side_timer_get_value();
	raspi_arm_side_timer_irq_disable();
	raspi_arm_side_timer_irq_clear();

	// The counter is decreasing, so to get the delay of the IRQ response we substract the value of the timer at the entry of this
	// function from the timer load value. Further, to account for the time needed to sample the timer, we take a second sample
	// and also substract the difference beween the two points
	timer_irq_delay = (raspi_arm_side_timer_get_load() - timerValue1) - (timerValue1 - timerValue2);

	//raspi_arm_side_timer_irq_enable();
	return 1;
}

void time_block_until(__snsec until __unused)
{
	until = until;
	return;
}

/* return ns since time_init() */
__nsec ukplat_monotonic_clock(void)
{
	return get_system_timer() * NSEC_PER_USEC;
}

/* return wall time in nsecs */
__nsec ukplat_wall_clock(void)
{
	return get_system_timer() * NSEC_PER_USEC;
}

/* must be called before interrupts are enabled */
void ukplat_time_init(void)
{
	int rc;

	irq_vector_init();

	raspi_arm_side_timer_init();

	rc = ukplat_irq_register(0, handle_timer_irq, NULL);
	if (rc < 0)
		UK_CRASH("Failed to register timer interrupt handler\n");
}

/**
 * Get System Timer's counter
 */
__u64 get_system_timer(void)
{
	__u32 h, l;
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
	return ((__u64)h << 32) | l;
}

__u32 get_timer_irq_delay(void)
{
	return timer_irq_delay;
}

void reset_timer_irq_delay(void)
{
	timer_irq_delay = 0;
}
