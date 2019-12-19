
.globl get_sp
get_sp:
    mov		x0, sp
    ret

.globl get_sp_start
get_sp_start:
    ldr     x1, =_start
    ret

.globl get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret

.globl get_spsel
get_spsel:
    mrs x0, SPSel
    ret

.globl get_sctlr_el1
get_sctlr_el1:
    mrs x0, sctlr_el1
    ret

.globl get_unikraft_text_size
get_unikraft_text_size:
	ldr x0, =_text
	ldr x1, =_ectors
	sub x0, x1, x0
	ret

.globl get_unikraft_data_size
get_unikraft_data_size:
	ldr x0, =_data
	ldr x1, =_edata
	sub x0, x1, x0
	ldr x1, =_tls_start
	ldr x2, =_etdata
	sub x1, x2, x1
	add x0, x0, x1
	ret

.globl get_unikraft_bss_size
get_unikraft_bss_size:
	ldr x0, =__bss_start
	ldr x1, =__bss_end
	sub x0, x1, x0
	ldr x1, =_etdata
	ldr x2, =_tls_end
	sub x1, x2, x1
	add x0, x0, x1
	ret
