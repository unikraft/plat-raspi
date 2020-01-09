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

#ifndef __RASPI_SYSREGS_H__
#define __RASPI_SYSREGS_H__

#define DEVICE_BASE		(0x3F000000)
#define MMIO_BASE		(DEVICE_BASE)

#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014))
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))
#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE+0x00200034))
#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE+0x00200038))
#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE+0x00200040))
#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE+0x00200044))
#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE+0x00200064))
#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE+0x00200068))
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C))

// ***************************************
// SCTLR_EL1, System Control Register (EL1), Page 2654 of AArch64-Reference-Manual.
// ***************************************
#define SCTLR_EL1_WFE_NORMAL				(1 << 18)
#define SCTLR_EL1_WFI_NORMAL				(1 << 16)
#define SCTLR_EL1_MMU_ENABLED               (1 << 0)

#define SCTLR_EL1_VALUE_MMU_DISABLED	(SCTLR_EL1_WFE_NORMAL | SCTLR_EL1_WFI_NORMAL)
#define SCTLR_EL1_VALUE_MMU_ENABLED		(SCTLR_EL1_WFE_NORMAL | SCTLR_EL1_WFI_NORMAL | SCTLR_EL1_MMU_ENABLED)
#define SCTLR_EL2_VALUE				(0)

// ***************************************
// HCR_EL2, Hypervisor Configuration Register (EL2), Page 2487 of AArch64-Reference-Manual.
// ***************************************

#define HCR_EL2_RW	    			(1 << 31)
#define HCR_EL2_IMO	    			(1 << 4)
#define HCR_EL2_SWIO	    		(1 << 1)
#define HCR_EL2_VALUE				(HCR_EL2_RW)

// ***************************************
// SCR_EL3, Secure Configuration Register (EL3), Page 2648 of AArch64-Reference-Manual.
// ***************************************

#define CPACR_EL1_TTA					(1 << 28)
#define CPACR_EL1_FPEN_TRAP_EL0_EL1  	(0 << 20)
#define CPACR_EL1_FPEN_TRAP_EL0		  	(1 << 20)
#define CPACR_EL1_FPEN_TRAP_NONE	  	(3 << 20)
#define CPACR_EL1_VALUE					(CPACR_EL1_FPEN_TRAP_NONE)

// ***************************************
// CPACR_EL1, Architectural Feature Access Control Register.
// ***************************************

#define SCR_RESERVED	    		(3 << 4)
#define SCR_RW				(1 << 10)
#define SCR_NS				(1 << 0)
#define SCR_VALUE	    	    	(SCR_RESERVED | SCR_RW | SCR_NS)

// ***************************************
// SPSR_EL3, Saved Program Status Register (EL3) Page 389 of AArch64-Reference-Manual.
// ***************************************

#define SPSR_MASK_ALL 		(7 << 6)
#define SPSR_ELxh			(5 << 0)
#define SPSR_EL2_VALUE		(SPSR_MASK_ALL | SPSR_ELxh)

#endif /* __RASPI_SYSREGS_H__ */
