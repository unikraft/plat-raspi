
#include <uk/plat/irq.h>

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
