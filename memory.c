/* SPDX-License-Identifier: ISC */
/* Copyright (c) 2015, IBM
 *           (c) 2020, NEC Laboratories Europe GmbH, NEC Corporation.
 * Author(s): Dan Williams <djwillia@us.ibm.com>
 *            Simon Kuenzer <simon.kuenzer@neclab.eu>
 *            Santiago Pagani <santiagopagani@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <uk/plat/memory.h>
#include <uk/plat/common/sections.h>
#include <uk/essentials.h>
#include <raspi/sysregs.h>

int ukplat_memregion_count(void)
{
	return 7;
}

int ukplat_memregion_get(int i, struct ukplat_memregion_desc *m)
{
	int ret;

	UK_ASSERT(m);

	switch (i) {
	case 0: /* stack */
		m->base  = (void *) 0;
		m->len   = (size_t) __TEXT;
		m->flags = (UKPLAT_MEMRF_RESERVED
			    | UKPLAT_MEMRF_READABLE
			    | UKPLAT_MEMRF_WRITABLE);
		ret = 0;
#if CONFIG_UKPLAT_MEMRNAME
		m->name  = "stack";
#endif
		break;
	case 1: /* text */
		m->base  = (void *) __TEXT;
		m->len   = (size_t) __ETEXT - (size_t) __TEXT;
		m->flags = (UKPLAT_MEMRF_RESERVED
			    | UKPLAT_MEMRF_READABLE);
#if CONFIG_UKPLAT_MEMRNAME
		m->name  = "text";
#endif
		ret = 0;
		break;
	case 2: /* rodata */
		m->base  = (void *) __RODATA;
		m->len   = (size_t) __ERODATA - (size_t) __RODATA;
		m->flags = (UKPLAT_MEMRF_RESERVED
			    | UKPLAT_MEMRF_READABLE);
#if CONFIG_UKPLAT_MEMRNAME
		m->name  = "rodata";
#endif
		ret = 0;
		break;
	case 3: /* ctors */
		m->base  = (void *) __CTORS;
		m->len   = (size_t) __ECTORS - (size_t) __CTORS;
		m->flags = (UKPLAT_MEMRF_RESERVED
			    | UKPLAT_MEMRF_READABLE);
#if CONFIG_UKPLAT_MEMRNAME
		m->name  = "ctors";
#endif
		ret = 0;
		break;
	case 4: /* data */
		m->base  = (void *) __DATA;
		m->len   = (size_t) __EDATA - (size_t) __DATA;
		m->flags = (UKPLAT_MEMRF_RESERVED
			    | UKPLAT_MEMRF_READABLE
			    | UKPLAT_MEMRF_WRITABLE);
#if CONFIG_UKPLAT_MEMRNAME
		m->name  = "data";
#endif
		ret = 0;
		break;
	case 5: /* bss */
		m->base  = (void *) __BSS_START;
		m->len   = (size_t) __END - (size_t) __BSS_START;
		m->flags = (UKPLAT_MEMRF_RESERVED
			    | UKPLAT_MEMRF_READABLE
			    | UKPLAT_MEMRF_WRITABLE);
#if CONFIG_UKPLAT_MEMRNAME
		m->name  = "bss";
#endif
		ret = 0;
		break;
	case 6: /* heap */
		m->base  = (void *) __END;
		m->len   = (size_t) (MMIO_BASE/2 - 1) - (size_t) __END;
		m->flags = UKPLAT_MEMRF_ALLOCATABLE;
#if CONFIG_UKPLAT_MEMRNAME
		m->name  = "heap";
#endif
		ret = 0;
		break;
	default:
		m->base  = __NULL;
		m->len   = 0;
		m->flags = 0x0;
#if CONFIG_UKPLAT_MEMRNAME
		m->name  = __NULL;
#endif
		ret = -1;
		break;
	}

	return ret;
}

int _ukplat_mem_mappings_init(void)
{
	return 0;
}

void ukplat_stack_set_current_thread(void *thread_addr __unused)
{
}
