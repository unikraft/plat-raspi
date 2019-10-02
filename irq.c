#include <uk/plat/irq.h>
#include <uk/print.h>
#include <raspi/irq.h>
#include <raspi/time.h>
#include <raspi/delays.h>
#include <raspi/serial_console.h>

int ukplat_irq_register(unsigned long irq, irq_handler_func_t func, void *arg)
{
	irq = irq;
	func = func;
	arg = arg;
	return 0;
}

int ukplat_irq_init(struct uk_alloc *a)
{
	a = a;
	return 0;
}



const char *entry_error_messages[] = {
	"SYNC_INVALID_EL3t",
	"IRQ_INVALID_EL3t",
	"FIQ_INVALID_EL3t",
	"ERROR_INVALID_EL3t",

	"SYNC_INVALID_EL3h",
	"IRQ_INVALID_EL3h",
	"FIQ_INVALID_EL3h",
	"ERROR_INVALID_EL3h",

	"SYNC_INVALID_EL2_64",
	"IRQ_INVALID_EL2_64",
	"FIQ_INVALID_EL2_64",
	"ERROR_INVALID_EL2_64",

	"SYNC_INVALID_EL2_32",
	"IRQ_INVALID_EL2_32",
	"FIQ_INVALID_EL2_32",
	"ERROR_INVALID_EL2_32",

	"SYNC_INVALID_EL2t",
	"IRQ_INVALID_EL2t",
	"FIQ_INVALID_EL2t",
	"ERROR_INVALID_EL2t",

	"SYNC_INVALID_EL2h",
	"IRQ_INVALID_EL2h",
	"FIQ_INVALID_EL2h",
	"ERROR_INVALID_EL2h",

	"SYNC_INVALID_EL1_64",
	"IRQ_INVALID_EL1_64",
	"FIQ_INVALID_EL1_64",
	"ERROR_INVALID_EL1_64",

	"SYNC_INVALID_EL1_32",
	"IRQ_INVALID_EL1_32",
	"FIQ_INVALID_EL1_32",
	"ERROR_INVALID_EL1_32",

	"SYNC_INVALID_EL1t",
	"IRQ_INVALID_EL1t",
	"FIQ_INVALID_EL1t",
	"ERROR_INVALID_EL1t",

	"SYNC_INVALID_EL1h",
	"IRQ_INVALID_EL1h",
	"FIQ_INVALID_EL1h",
	"ERROR_INVALID_EL1h",

	"SYNC_INVALID_EL0_64",
	"IRQ_INVALID_EL0_64",
	"FIQ_INVALID_EL0_64",
	"ERROR_INVALID_EL0_64",

	"SYNC_INVALID_EL0_32",
	"IRQ_INVALID_EL0_32",
	"FIQ_INVALID_EL0_32",
	"ERROR_INVALID_EL0_32",

	"SYNC_INVALID_EL0t",
	"IRQ_INVALID_EL0t",
	"FIQ_INVALID_EL0t",
	"ERROR_INVALID_EL0t",

	"SYNC_INVALID_EL0h",
	"IRQ_INVALID_EL0h",
	"FIQ_INVALID_EL0h",
	"ERROR_INVALID_EL0h",

	"SYNC_INVALID_ELN_64",
	"IRQ_INVALID_ELN_64",
	"FIQ_INVALID_ELN_64",
	"ERROR_INVALID_ELN_64",

	"SYNC_INVALID_ELN_32",
	"IRQ_INVALID_ELN_32",
	"FIQ_INVALID_ELN_32",
	"ERROR_INVALID_ELN_32"
};

void enable_interrupt_controller(void)
{
	*DISABLE_IRQS_2 = 0xFFFFFFFF;
	*DISABLE_IRQS_1 = ~SYSTEM_TIMER_IRQ_1;
	*ENABLE_IRQS_1 = SYSTEM_TIMER_IRQ_1;
}

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address)
{
	uk_pr_debug("%s, ESR: %lu, address: %lu\n", entry_error_messages[type], esr, address);
}

void handle_irq(void)
{
	unsigned int irq = *IRQ_PENDING_1;
	switch (irq) {
		case (SYSTEM_TIMER_IRQ_1):
			handle_timer_irq();
			break;
		default:
			uk_pr_debug("Unknown pending irq: %u\n", irq);
	}
}
