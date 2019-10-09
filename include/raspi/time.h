
#ifndef __RASPI_TIME_H__
#define __RASPI_TIME_H__

#define TIMER_CS        ((volatile __u32 *)(PBASE+0x00003000))
#define TIMER_CLO       ((volatile __u32 *)(PBASE+0x00003004))
#define TIMER_CHI       ((volatile __u32 *)(PBASE+0x00003008))
#define TIMER_C0        ((volatile __u32 *)(PBASE+0x0000300C))
#define TIMER_C1        ((volatile __u32 *)(PBASE+0x00003010))
#define TIMER_C2        ((volatile __u32 *)(PBASE+0x00003014))
#define TIMER_C3        ((volatile __u32 *)(PBASE+0x00003018))

#define TIMER_CS_M0	(1 << 0)
#define TIMER_CS_M1	(1 << 1)
#define TIMER_CS_M2	(1 << 2)
#define TIMER_CS_M3	(1 << 3)

void handle_timer_irq(void);
void generic_timer_setup_next_interrupt(__u64 delta);

#endif /* __RASPI_TIME_H__ */