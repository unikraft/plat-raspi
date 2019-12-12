#ifndef _RASPI_INFO_H
#define _RASPI_INFO_H

unsigned long get_el ( void );
unsigned long get_spsel ( void );
unsigned long get_sctlr_el1 ( void );
unsigned long get_unikraft_text_size ( void );
unsigned long get_unikraft_data_size ( void );
unsigned long get_unikraft_bss_size ( void );

#endif
