
#ifndef __RASPI_IRQ_H__
#define __RASPI_IRQ_H__

#define PBASE (0x3F000000)

#define IRQ_BASIC_PENDING	((volatile __u32 *)(PBASE+0x0000B200))
#define IRQ_PENDING_1		((volatile __u32 *)(PBASE+0x0000B204))
#define IRQ_PENDING_2		((volatile __u32 *)(PBASE+0x0000B208))
#define FIQ_CONTROL			((volatile __u32 *)(PBASE+0x0000B20C))
#define ENABLE_IRQS_1		((volatile __u32 *)(PBASE+0x0000B210))
#define ENABLE_IRQS_2		((volatile __u32 *)(PBASE+0x0000B214))
#define ENABLE_BASIC_IRQS	((volatile __u32 *)(PBASE+0x0000B218))
#define DISABLE_IRQS_1		((volatile __u32 *)(PBASE+0x0000B21C))
#define DISABLE_IRQS_2		((volatile __u32 *)(PBASE+0x0000B220))
#define DISABLE_BASIC_IRQS	((volatile __u32 *)(PBASE+0x0000B224))

#define SYSTEM_TIMER_IRQ_0	(1 << 0)
#define SYSTEM_TIMER_IRQ_1	(1 << 1)
#define SYSTEM_TIMER_IRQ_2	(1 << 2)
#define SYSTEM_TIMER_IRQ_3	(1 << 3)
#define USB_IRQ				(1 << 9)


void enable_interrupt_controller( void );

void irq_vector_init( void );
void enable_irq( void );
void disable_irq( void );
int get_el ( void );
int get_spsel ( void );

void handle_timer_irq(void);

#endif /* __RASPI_IRQ_H__ */