
#ifndef __RASPI_TIME_H__
#define __RASPI_TIME_H__

#define RASPI_SYS_TIMER_BASE		(0x3F000000)
#define RASPI_SYS_TIMER_CS			((volatile __u32 *)(RASPI_SYS_TIMER_BASE+0x00003000))
#define RASPI_SYS_TIMER_CLO			((volatile __u32 *)(RASPI_SYS_TIMER_BASE+0x00003004))
#define RASPI_SYS_TIMER_CHI			((volatile __u32 *)(RASPI_SYS_TIMER_BASE+0x00003008))
#define RASPI_SYS_TIMER_C0			((volatile __u32 *)(RASPI_SYS_TIMER_BASE+0x0000300C))
#define RASPI_SYS_TIMER_C1			((volatile __u32 *)(RASPI_SYS_TIMER_BASE+0x00003010))
#define RASPI_SYS_TIMER_C2			((volatile __u32 *)(RASPI_SYS_TIMER_BASE+0x00003014))
#define RASPI_SYS_TIMER_C3			((volatile __u32 *)(RASPI_SYS_TIMER_BASE+0x00003018))

#define RASPI_SYS_TIMER_CS_M0		(1 << 0)
#define RASPI_SYS_TIMER_CS_M1		(1 << 1)
#define RASPI_SYS_TIMER_CS_M2		(1 << 2)
#define RASPI_SYS_TIMER_CS_M3		(1 << 3)


//#define RASPI_ARM_TIMER_BASE		(0x40000000)
//#define RASPI_ARM_TIMER_CTL			((volatile __u32 *)(RASPI_ARM_TIMER_BASE+0x00))
//#define RASPI_ARM_TIMER_PRESCALER	((volatile __u32 *)(RASPI_ARM_TIMER_BASE+0x08))
//#define RASPI_ARM_GPU_IRQ_ROUTNG	((volatile __u32 *)(RASPI_ARM_TIMER_BASE+0x0C))
//#define RASPI_ARM_TIMER_LOW			((volatile __u32 *)(RASPI_ARM_TIMER_BASE+0x1C))
//#define RASPI_ARM_TIMER_HIGH		((volatile __u32 *)(RASPI_ARM_TIMER_BASE+0x20))
//#define RASPI_ARM_C0_TIMER_IRQ_CTL	((volatile __u32 *)(RASPI_ARM_TIMER_BASE+0x40))


#define RASPI_ARM_SIDE_TIMER_BASE		(0x3F00B000)
#define RASPI_ARM_SIDE_TIMER_LOAD		((volatile __u32 *)(RASPI_ARM_SIDE_TIMER_BASE+0x400))
#define RASPI_ARM_SIDE_TIMER_VALUE		((volatile __u32 *)(RASPI_ARM_SIDE_TIMER_BASE+0x404))
#define RASPI_ARM_SIDE_TIMER_CTL		((volatile __u32 *)(RASPI_ARM_SIDE_TIMER_BASE+0x408))
#define RASPI_ARM_SIDE_TIMER_IRQ_CLEAR	((volatile __u32 *)(RASPI_ARM_SIDE_TIMER_BASE+0x40C))
#define RASPI_ARM_SIDE_TIMER_RAW_IRQ	((volatile __u32 *)(RASPI_ARM_SIDE_TIMER_BASE+0x410))
#define RASPI_ARM_SIDE_TIMER_MASK_IRQ	((volatile __u32 *)(RASPI_ARM_SIDE_TIMER_BASE+0x414))
#define RASPI_ARM_SIDE_TIMER_RELOAD		((volatile __u32 *)(RASPI_ARM_SIDE_TIMER_BASE+0x418))
#define RASPI_ARM_SIDE_TIMER_PREDIVIDER	((volatile __u32 *)(RASPI_ARM_SIDE_TIMER_BASE+0x41C))
#define RASPI_ARM_SIDE_TIMER_FREE		((volatile __u32 *)(RASPI_ARM_SIDE_TIMER_BASE+0x420))
#define RASPI_ARM_SIDE_TIMER_CTL_FREE_COUNTER_BIT	(1 << 9)
#define RASPI_ARM_SIDE_TIMER_CTL_ENABLE_BIT			(1 << 7)
#define RASPI_ARM_SIDE_TIMER_CTL_IRQ_BIT			(1 << 5)
#define RASPI_ARM_SIDE_TIMER_CTL_BITS_BIT			(1 << 1)


#define raspi_arm_side_timer_get_value()	(*RASPI_ARM_SIDE_TIMER_VALUE)
#define raspi_arm_side_timer_get_load()		(*RASPI_ARM_SIDE_TIMER_LOAD)
#define raspi_arm_side_timer_irq_enable()	{*RASPI_ARM_SIDE_TIMER_CTL = *RASPI_ARM_SIDE_TIMER_CTL | RASPI_ARM_SIDE_TIMER_CTL_IRQ_BIT; }
#define raspi_arm_side_timer_irq_disable()	{*RASPI_ARM_SIDE_TIMER_CTL = *RASPI_ARM_SIDE_TIMER_CTL & ~RASPI_ARM_SIDE_TIMER_CTL_IRQ_BIT; }
#define raspi_arm_side_timer_irq_clear()	{*RASPI_ARM_SIDE_TIMER_IRQ_CLEAR = 1; }


__u64 get_system_timer(void);
void handle_timer_irq(void);
void generic_timer_setup_next_interrupt(__u64 delta);

#endif /* __RASPI_TIME_H__ */