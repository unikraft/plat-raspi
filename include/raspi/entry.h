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

#ifndef __RASPI_ENTRY_H__
#define __RASPI_ENTRY_H__

#define S_FRAME_SIZE			256 		// Size of all saved registers 

#define SYNC_INVALID_EL3t		0 
#define IRQ_INVALID_EL3t		1 
#define FIQ_INVALID_EL3t		2 
#define ERROR_INVALID_EL3t		3 

#define SYNC_INVALID_EL3h		4 
#define IRQ_INVALID_EL3h		5 
#define FIQ_INVALID_EL3h		6 
#define ERROR_INVALID_EL3h		7 

#define SYNC_INVALID_EL2_64	    8 
#define IRQ_INVALID_EL2_64	    9 
#define FIQ_INVALID_EL2_64		10 
#define ERROR_INVALID_EL2_64	11 

#define SYNC_INVALID_EL2_32		12 
#define IRQ_INVALID_EL2_32		13 
#define FIQ_INVALID_EL2_32		14 
#define ERROR_INVALID_EL2_32	15

#define SYNC_INVALID_EL2t		16 
#define IRQ_INVALID_EL2t		17
#define FIQ_INVALID_EL2t		18
#define ERROR_INVALID_EL2t		19

#define SYNC_INVALID_EL2h		20
#define IRQ_INVALID_EL2h		21
#define FIQ_INVALID_EL2h		22
#define ERROR_INVALID_EL2h		23

#define SYNC_INVALID_EL1_64	    24
#define IRQ_INVALID_EL1_64	    25
#define FIQ_INVALID_EL1_64		26 
#define ERROR_INVALID_EL1_64	27

#define SYNC_INVALID_EL1_32		28
#define IRQ_INVALID_EL1_32		29 
#define FIQ_INVALID_EL1_32		30 
#define ERROR_INVALID_EL1_32	31 

#define SYNC_INVALID_EL1t		32
#define IRQ_INVALID_EL1t		33
#define FIQ_INVALID_EL1t		34
#define ERROR_INVALID_EL1t		35

#define SYNC_INVALID_EL1h		36
#define IRQ_INVALID_EL1h		37
#define FIQ_INVALID_EL1h		38
#define ERROR_INVALID_EL1h		49

#define SYNC_INVALID_EL0_64	    40
#define IRQ_INVALID_EL0_64	    41
#define FIQ_INVALID_EL0_64		42 
#define ERROR_INVALID_EL0_64	43 

#define SYNC_INVALID_EL0_32		44 
#define IRQ_INVALID_EL0_32		45 
#define FIQ_INVALID_EL0_32		46 
#define ERROR_INVALID_EL0_32	47

#define SYNC_INVALID_EL0t		48
#define IRQ_INVALID_EL0t		49
#define FIQ_INVALID_EL0t		50
#define ERROR_INVALID_EL0t		51

#define SYNC_INVALID_EL0h		52
#define IRQ_INVALID_EL0h		53
#define FIQ_INVALID_EL0h		54
#define ERROR_INVALID_EL0h		55

#define SYNC_INVALID_ELN_64	    56
#define IRQ_INVALID_ELN_64	    57
#define FIQ_INVALID_ELN_64		58 
#define ERROR_INVALID_ELN_64	59 

#define SYNC_INVALID_ELN_32		60 
#define IRQ_INVALID_ELN_32		61 
#define FIQ_INVALID_ELN_32		62 
#define ERROR_INVALID_ELN_32	63 

#endif /* __RASPI_ENTRY_H__ */
