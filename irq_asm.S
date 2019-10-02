.globl irq_vector_init
irq_vector_init:
	//adr	x0, vectors_el3		// load VBAR_EL1 with virtual
	//msr	vbar_el3, x0		// vector table address
	adr	x0, vectors_el2		// load VBAR_EL1 with virtual
	msr	vbar_el2, x0		// vector table address
	//adr	x0, vectors_el1		// load VBAR_EL1 with virtual
	//msr	vbar_el1, x0		// vector table address
	//adr	x0, vectors_el0		// load VBAR_EL1 with virtual
	//msr	vbar_el0, x0		// vector table address
	ret

.globl enable_irq
enable_irq:
	msr    daifclr, #2 
	ret

.globl disable_irq
disable_irq:
	msr	daifset, #2
	ret

.globl get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret