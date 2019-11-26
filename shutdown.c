
#include <errno.h>
#include <uk/plat/common/cpu.h>
#include <uk/plat/common/irq.h>
#include <uk/plat/bootstrap.h>

static void cpu_halt(void) __noreturn;

void ukplat_terminate(enum ukplat_gstate request __unused)
{
	cpu_halt();
}

static void cpu_halt(void)
{
	__CPU_HALT();
}

int ukplat_suspend(void)
{
	return -EBUSY;
}
