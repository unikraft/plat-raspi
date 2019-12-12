#include <stdlib.h>
#include <uk/plat/irq.h>
#include <uk/print.h>
#include <uk/essentials.h>
#include <raspi/irq.h>
#include <raspi/time.h>
#include <raspi/raspi_info.h>
#include <arm/time.h>

static irq_handler_func_t irq_handlers[IRQS_MAX];

int ukplat_irq_register(unsigned long irq, irq_handler_func_t func, void *arg __unused)
{
	switch (irq) {
		case IRQ_ID_ARM_GENERIC_TIMER:
			break;
		case IRQ_ID_RASPI_ARM_SIDE_TIMER:
			*ENABLE_BASIC_IRQS = *ENABLE_BASIC_IRQS | IRQS_BASIC_ARM_TIMER_IRQ;
			raspi_arm_side_timer_irq_clear();
			raspi_arm_side_timer_irq_enable();
			break;
		default:
			// Unsupported IRQ
			uk_pr_crit("ukplat_irq_register: Unsupported IRQ\n");
			return -1;
	}

	irq_handlers[irq] = func;
	return 0;
}

int ukplat_irq_init(struct uk_alloc *a __unused)
{
	for (unsigned int i = 0; i < IRQS_MAX; i++) {
		irq_handlers[i] = NULL;
	}
	*DISABLE_BASIC_IRQS = 0xFFFFFFFF;
	*DISABLE_IRQS_1 = 0xFFFFFFFF;
	*DISABLE_IRQS_2 = 0xFFFFFFFF;
	irq_vector_init();
	enable_irq();
	return 0;
}

void show_invalid_entry_message(int type)
{
	uk_pr_debug("IRQ: %d\n", type);
}

void show_invalid_entry_message_el1_sync(uint64_t esr_el, uint64_t far_el)
{
	uk_pr_debug("ESR_EL1: %lx, FAR_EL1: %lx, SCTLR_EL1:%lx\n", esr_el, far_el, get_sctlr_el1());
}

void ukplat_irq_handle(void)
{
	__u32 irq_bits = *IRQ_BASIC_PENDING & *ENABLE_BASIC_IRQS;
	if ((irq_bits & IRQS_BASIC_ARM_TIMER_IRQ) && irq_handlers[IRQ_ID_RASPI_ARM_SIDE_TIMER]) {
		irq_handlers[IRQ_ID_RASPI_ARM_SIDE_TIMER](NULL);
		return;
	}

	if ((get_el0(cntv_ctl) & GT_TIMER_IRQ_STATUS) && irq_handlers[IRQ_ID_ARM_GENERIC_TIMER]) {
		irq_handlers[IRQ_ID_ARM_GENERIC_TIMER](NULL);
		return;
	}

	/*
	 * Just warn about unhandled interrupts. We do this to
	 * (1) compensate potential spurious interrupts of
	 * devices, and (2) to minimize impact on drivers that share
	 * one interrupt line that would then stay disabled.
	 */
	uk_pr_crit("ukplat_irq_handle: Unhandled IRQ\n");
	uk_pr_crit("IRQ_BASIC_PENDING: %u\n", *IRQ_BASIC_PENDING);
	uk_pr_crit("IRQ_PENDING_1: %u\n", *IRQ_PENDING_1);
	uk_pr_crit("IRQ_PENDING_2: %u\n", *IRQ_PENDING_2);
	uk_pr_crit("ENABLE_BASIC_IRQS: %u\n", *ENABLE_BASIC_IRQS);
	uk_pr_crit("ENABLE_IRQS_1: %u\n", *ENABLE_IRQS_1);
	uk_pr_crit("ENABLE_IRQS_2: %u\n", *ENABLE_IRQS_2);
	uk_pr_crit("DISABLE_BASIC_IRQS: %u\n", *DISABLE_BASIC_IRQS);
	uk_pr_crit("DISABLE_IRQS_1: %u\n", *DISABLE_IRQS_1);
	uk_pr_crit("DISABLE_IRQS_2: %u\n", *DISABLE_IRQS_2);
	uk_pr_crit("get_el0(cntv_ctl): %lu\n", get_el0(cntv_ctl));
	uk_pr_crit("irq_handlers[IRQ_ID_ARM_GENERIC_TIMER]: %lu\n", (unsigned long)irq_handlers[IRQ_ID_ARM_GENERIC_TIMER]);
	uk_pr_crit("irq_handlers[IRQ_ID_RASPI_ARM_SIDE_TIMER]: %lu\n", (unsigned long)irq_handlers[IRQ_ID_RASPI_ARM_SIDE_TIMER]);
	while(1);
}
