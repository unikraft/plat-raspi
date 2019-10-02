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
#include <cpu.h>
#include <irq.h>

typedef __u64 uint64_t;
typedef __u32 uint32_t;

#include <raspi/delays.h>
#include <raspi/irq.h>
#include <raspi/time.h>
#include <uk/print.h>


/* Bits definition of cntv_ctl_el0 register */
#define GT_TIMER_ENABLE        0x01
#define GT_TIMER_MASK_IRQ      0x02
#define GT_TIMER_IRQ_STATUS    0x04

#define  cntv_ctl_get()  SYSREG_READ32(cntv_ctl_el0)
#define  cntv_ctl_set(val) SYSREG_WRITE32(cntv_ctl_el0, val)

static inline void generic_timer_enable(void)
{
	cntv_ctl_set(cntv_ctl_get() | GT_TIMER_ENABLE);

	/* Ensure the write of sys register is visible */
	isb();
}

static inline void generic_timer_disable(void)
{
	cntv_ctl_set(cntv_ctl_get() & (~GT_TIMER_ENABLE));

	/* Ensure the write of sys register is visible */
	isb();
}

static inline void generic_timer_mask_irq(void)
{
	cntv_ctl_set(cntv_ctl_get() | GT_TIMER_MASK_IRQ);

	/* Ensure the write of sys register is visible */
	isb();
}

static inline void generic_timer_unmask_irq(void)
{
	cntv_ctl_set(cntv_ctl_get() & (~GT_TIMER_MASK_IRQ));

	/* Ensure the write of sys register is visible */
	isb();
}

static inline void generic_timer_update_compare(uint64_t new_val)
{
	SYSREG_WRITE64(cntv_cval_el0, new_val);

	/* Ensure the write of sys register is visible */
	isb();
}

static inline uint32_t read_freq_from_sysreg(void)
{
	return SYSREG_READ32(cntfrq_el0);
}

#ifdef CONFIG_ARM64_ERRATUM_858921
/*
 * The errata #858921 describes that Cortex-A73 (r0p0 - r0p2) counter
 * read can return a wrong value when the counter crosses a 32bit boundary.
 * But newer Cortex-A73 are not affected.
 *
 * The workaround involves performing the read twice, compare bit[32] of
 * the two read values. If bit[32] is different, keep the first value,
 * otherwise keep the second value.
 */
static uint64_t generic_timer_get_ticks(void)
{
	uint64_t val_1st, val_2nd;

	val_1st = SYSREG_READ64(cntvct_el0);
	val_2nd = SYSREG_READ64(cntvct_el0);
	return (((val_1st ^ val_2nd) >> 32) & 1) ? val_1st : val_2nd;
}
#else
static inline uint64_t generic_timer_get_ticks(void)
{
	return SYSREG_READ64(cntvct_el0);
}
#endif

//static uint32_t counter_freq;


/* Shift factor for converting ticks to ns */
static uint8_t counter_shift_to_ns;

/* Shift factor for converting ns to ticks */
static uint8_t counter_shift_to_tick;

/* Multiplier for converting counter ticks to nsecs */
static uint32_t ns_per_tick;

/* Multiplier for converting nsecs to counter ticks */
static uint32_t tick_per_ns;

/*
 * The maximum time range in seconds which can be converted by multiplier
 * and shift factors. This will guarantee the converted value not to exceed
 * 64-bit unsigned integer. Increase the time range will reduce the accuracy
 * of conversion, because we will get smaller multiplier and shift factors.
 * In this case, we selected 3600s as the time range.
 */
#define __MAX_CONVERT_SECS	(3600UL)
#define __MAX_CONVERT_NS	(3600UL*NSEC_PER_SEC)
//static uint64_t max_convert_ticks;

/* How many nanoseconds per second */
#define NSEC_PER_SEC ukarch_time_sec_to_nsec(1)

static inline uint64_t ticks_to_ns(uint64_t ticks)
{
	//UK_ASSERT(ticks <= max_convert_ticks);

	return (ns_per_tick * ticks) >> counter_shift_to_ns;
}

static inline uint64_t ns_to_ticks(uint64_t ns)
{
	//UK_ASSERT(ns <= __MAX_CONVERT_NS);

	return (tick_per_ns * ns) >> counter_shift_to_tick;
}

/*
 * Calculate multiplier/shift factors for scaled math.
 */
/*static void calculate_mult_shift(uint32_t *mult, uint8_t *shift,
		uint64_t from, uint64_t to)
{
	uint64_t tmp;
	uint32_t sft, sftacc = 32;

	tmp = ((uint64_t)__MAX_CONVERT_SECS * from) >> 32;
	while (tmp) {
		tmp >>= 1;
		sftacc--;
	}


	for (sft = 32; sft > 0; sft--) {
		tmp = (uint64_t) to << sft;

		tmp += from / 2;
		tmp /= from;
		if ((tmp >> sftacc) == 0)
			break;
	}
	*mult = tmp;
	*shift = sft;
}*/

/*static uint32_t generic_timer_get_frequency(void)
{
	return read_freq_from_sysreg();
}*/

/*
 * monotonic_clock(): returns # of nanoseconds passed since
 * generic_timer_time_init()
 */
/*static __nsec generic_timer_monotonic(void)
{
	return (__nsec)ticks_to_ns(generic_timer_get_ticks());
}*/

/*
 * Return epoch offset (wall time offset to monotonic clock start).
 */
/*static uint64_t generic_timer_epochoffset(void)
{
	return 0;
}*/

/*
 * Returns early if any interrupts are serviced, or if the requested delay is
 * too short. Must be called with interrupts disabled, will enable interrupts
 * "atomically" during idle loop.
 *
 * This function must be called only from the scheduler. It will screw
 * your system if you do otherwise. And, there is no reason you
 * actually want to use it anywhere else. THIS IS NOT A YIELD or any
 * kind of mutex_lock. It will simply halt the cpu, not allowing any
 * other thread to execute.
 */
/*static void generic_timer_setup_next_interrupt(uint64_t delta)
{
	uint64_t until_ticks;

	UK_ASSERT(ukplat_lcpu_irqs_disabled());

	until_ticks = generic_timer_get_ticks() + ns_to_ticks(delta);

	generic_timer_update_compare(until_ticks);
	generic_timer_unmask_irq();
}*/

void generic_timer_setup_next_interrupt(uint32_t delta) {
	*TIMER_C1 = *TIMER_CLO + delta;
}

/*static int generic_timer_init(void)
{
	counter_freq = generic_timer_get_frequency();

	calculate_mult_shift(&ns_per_tick, &counter_shift_to_ns,
				counter_freq, NSEC_PER_SEC);

	UK_BUGON(!ns_per_tick);

	calculate_mult_shift(&tick_per_ns, &counter_shift_to_tick,
				NSEC_PER_SEC, counter_freq);

	UK_BUGON(!tick_per_ns);

	max_convert_ticks = __MAX_CONVERT_SECS*counter_freq;

	return 0;
}*/

/*static int generic_timer_irq_handler(void *arg __unused)
{
	generic_timer_mask_irq();
	uk_pr_debug("Timer value: %lu\n", get_system_timer());

	generic_timer_setup_next_interrupt(NSEC_PER_SEC);

	return 1;
}*/

void time_block_until(__snsec until)
{
	until = until;
	return;
}

/* return ns since time_init() */
__nsec ukplat_monotonic_clock(void)
{
	return 42;
	//return generic_timer_monotonic();
}

/* return wall time in nsecs */
__nsec ukplat_wall_clock(void)
{
	return 43;
	//return generic_timer_monotonic() + generic_timer_epochoffset();
}






/* must be called before interrupts are enabled */
void ukplat_time_init(void)
{
	/*int rc, irq;

	rc = generic_timer_init();
	if (rc < 0)
		UK_CRASH("Failed to initialize platform time\n");

	irq = 0;
	if (irq < 0 || irq >= __MAX_IRQ)
		UK_CRASH("Failed to translate IRQ number\n");

	rc = ukplat_irq_register(irq, generic_timer_irq_handler, NULL);
	if (rc < 0)
		UK_CRASH("Failed to register timer interrupt handler\n");

	generic_timer_mask_irq();

	generic_timer_enable();

	generic_timer_setup_next_interrupt(NSEC_PER_SEC);
	*/





	/*irq_vector_init();
	uk_pr_debug("Initial CLO value: %u\n", (__u32)*TIMER_CLO);
	generic_timer_setup_next_interrupt(1000000);
	uk_pr_debug("Compaer C1 value:  %u\n", (__u32)*TIMER_C1);
	enable_interrupt_controller();
	enable_irq();*/
}


void handle_timer_irq( void ) 
{
	__u32 timerValue = *TIMER_CLO;
	uk_pr_debug("Timer IRQ delay: %u\n", timerValue - *TIMER_C1);
	*TIMER_CS = TIMER_CS_M1;
	generic_timer_setup_next_interrupt(1000000);
}
