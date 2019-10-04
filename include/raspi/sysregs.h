#ifndef _SYSREGS_H
#define _SYSREGS_H

// ***************************************
// SCTLR_EL1, System Control Register (EL1), Page 2654 of AArch64-Reference-Manual.
// ***************************************

//#define SCTLR_EL1_RESERVED                  (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
//#define SCTLR_EL1_EE_LITTLE_ENDIAN          (0 << 25)
//#define SCTLR_EL1_EOE_LITTLE_ENDIAN         (0 << 24)
#define SCTLR_EL1_WFE_NORMAL				(1 << 18)
#define SCTLR_EL1_WFI_NORMAL				(1 << 16)
//#define SCTLR_EL1_I_CACHE_DISABLED          (0 << 12)
//#define SCTLR_EL1_D_CACHE_DISABLED          (0 << 2)
//#define SCTLR_EL1_MMU_DISABLED              (0 << 0)
//#define SCTLR_EL1_MMU_ENABLED               (1 << 0)

//#define SCTLR_VALUE_MMU_DISABLED	(SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)
#define SCTLR_EL1_VALUE				(SCTLR_EL1_WFE_NORMAL | SCTLR_EL1_WFI_NORMAL)
#define SCTLR_EL2_VALUE				(0)

// ***************************************
// HCR_EL2, Hypervisor Configuration Register (EL2), Page 2487 of AArch64-Reference-Manual.
// ***************************************

#define HCR_EL2_RW	    			(1 << 31)
#define HCR_EL2_IMO	    			(1 << 4)
#define HCR_EL2_SWIO	    		(1 << 1)
//#define HCR_EL2_VALUE				(HCR_EL2_RW | HCR_EL2_SWIO | HCR_EL2_SWIO)
#define HCR_EL2_VALUE				(HCR_EL2_RW | HCR_EL2_SWIO)

// ***************************************
// SCR_EL3, Secure Configuration Register (EL3), Page 2648 of AArch64-Reference-Manual.
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

#endif
