
#include <uk/plat/memory.h>
#include <uk/essentials.h>

int ukplat_memregion_count(void)
{
	return 0;
}

int ukplat_memregion_get(int i __unused, struct ukplat_memregion_desc *m __unused)
{
	return 0;
}

int _ukplat_mem_mappings_init(void)
{
	return 0;
}

void ukplat_stack_set_current_thread(void *thread_addr __unused)
{
}
