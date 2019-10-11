#include <stdlib.h>
#include <uk/plat/irq.h>
#include <uk/print.h>
#include <uk/essentials.h>
#include <raspi/irq.h>
#include <raspi/time.h>

static irq_handler_func_t irq_handlers[IRQS_MAX];

int ukplat_irq_register(unsigned long irq, irq_handler_func_t func, void *arg __unused)
{
	if (irq == IRQ_ID_ARM_TIMER) {
		irq_handlers[IRQ_ID_ARM_TIMER] = func;
		*ENABLE_BASIC_IRQS = *ENABLE_BASIC_IRQS | IRQS_BASIC_ARM_TIMER_IRQ;
		raspi_arm_side_timer_irq_clear();
		raspi_arm_side_timer_irq_enable();

		return 0;
	}

	// Unsupported IRQ
	uk_pr_crit("ukplat_irq_register: Unsupported IRQ\n");
	return -1;
}

int ukplat_irq_init(struct uk_alloc *a __unused)
{
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

void ukplat_irq_handle(void)
{
	__u32 irq_bits = *IRQ_BASIC_PENDING & *ENABLE_BASIC_IRQS;
	if (irq_bits & IRQS_BASIC_ARM_TIMER_IRQ) {
		irq_handlers[IRQ_ID_ARM_TIMER](NULL);
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
	while(1);
}
