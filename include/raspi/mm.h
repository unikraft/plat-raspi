/*
 * MIT License
 *
 * Copyright (c) 2018, Sergey Matyukevich <https://github.com/s-matyukevich/raspberry-pi-os>
 *           (c) 2020, Santiago Pagani <santiagopagani@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __RASPI_MM_H__
#define __RASPI_MM_H__

/* From https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson06/rpi-os.md:
 * We need just one PGD and one PUD to map the whole RPi memory, and both PGD and PUD will contain a single descriptor.
 * If we have a single PUD entry there also must be a single PMD table, to which this entry will point.
 * Single PMD entry covers 2 MB, there are 512 items in a PMD, so in total the whole PMD table covers the same 1 GB of
 * memory that is covered by a single PUD descriptor.
 * We need to map 1 GB region of memory, which is a multiple of 2 MB, therefore we can use section mapping, and hence we
 * don't need PTE at all.
 * Finally, that means we need three 4 kB pages for the page tables: one for PGD, PUD and PMD.
 */

#define VA_START 			0
#define PHYS_MEMORY_SIZE 	0x40000000

#define PAGE_MASK			0xFFFFFFFFFFFFF000
#define PAGE_SHIFT	 		12
#define TABLE_SHIFT 		9
#define SECTION_SHIFT		(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE   		(1 << PAGE_SHIFT)	
#define SECTION_SIZE		(1 << SECTION_SHIFT)	

#define LOW_MEMORY          (2 * SECTION_SIZE)
#define HIGH_MEMORY         DEVICE_BASE

#define PAGING_MEMORY 		(HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES 		(PAGING_MEMORY/PAGE_SIZE)

#define PTRS_PER_TABLE		(1 << TABLE_SHIFT)

#define PGD_SHIFT			PAGE_SHIFT + 3*TABLE_SHIFT
#define PUD_SHIFT			PAGE_SHIFT + 2*TABLE_SHIFT
#define PMD_SHIFT			PAGE_SHIFT + TABLE_SHIFT

#define PG_DIR_SIZE			(3 * PAGE_SIZE)

#endif /* __RASPI_MM_H__ */
